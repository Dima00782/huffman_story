#ifndef ENCODING_HUFFMAN_ENCODING_H_
#define ENCODING_HUFFMAN_ENCODING_H_

#include <cassert>
#include <istream>
#include <iterator>
#include <memory>
#include <ostream>
#include <stack>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "bit_io/bit_reader.h"
#include "bit_io/bit_writer.h"
#include "encoding/byte_streams_adapters/byte_aligned_bit_reader.h"
#include "encoding/byte_streams_adapters/byte_aligned_bit_writer.h"
#include "encoding/huffman_tree/huffman_tree_builder.h"
#include "encoding/text_splitter/text_splitter.h"
#include "letter/letter.h"

namespace {

constexpr bool kInnerNodeBitLabel = false;
constexpr bool kLeafNodeBitLabel = true;
constexpr bool kTurnLeftBitLabel = false;
constexpr bool kTurnRightBitLabel = true;
constexpr uint8_t kNumBitsForKeySize = 8u;

template <typename LetterType>
std::unordered_map<LetterType, std::vector<bool>> BuildCodesMap(
    huffman_tree::TreeNode<LetterType>* root) {
  assert(root);
  std::unordered_map<LetterType, std::vector<bool>> codes;

  if (root->isLeaf()) {
    codes[root->key_] = std::vector<bool>(1, kTurnLeftBitLabel);
    return codes;
  }

  struct NodeWithCode {
    huffman_tree::TreeNode<LetterType>* node;
    std::vector<bool> code;
  };

  std::stack<NodeWithCode> stack;
  stack.push(NodeWithCode{root, std::vector<bool>()});

  while (!stack.empty()) {
    const auto current = stack.top();
    stack.pop();

    if (current.node->isLeaf()) {
      codes[current.node->key_] = current.code;
      continue;
    }

    assert(current.node->isInner());

    {
      auto code = current.code;
      code.push_back(kTurnLeftBitLabel);
      stack.push(NodeWithCode{current.node->left_.get(), std::move(code)});
    }

    {
      auto code = current.code;
      code.push_back(kTurnRightBitLabel);
      stack.push(NodeWithCode{current.node->right_.get(), std::move(code)});
    }
  }

  return codes;
}

void ResetInputStream(std::istream& input) {
  input.clear();
  input.seekg(0);
}

}  // namespace

namespace encoding {
template <letter::LetterConfig Config>
class HuffmanEncoder {
 public:
  using LetterType = Config::LetterType;
  using TreeNode = huffman_tree::TreeNode<LetterType>;

  HuffmanEncoder(std::shared_ptr<Config> config,
                 std::shared_ptr<std::istream> input,
                 std::shared_ptr<std::ostream> output)
      : output_(std::make_shared<byte_adapters::ByteAlignedBitWriter>(
            std::move(output))),
        config_(std::move(config)) {
    auto letter_frequencies = CountLetterFrequencies(input);
    auto root = huffman_tree::BuildHuffmanTree<LetterType>(letter_frequencies);
    WriteTreeInPrefixForm(root.get());
    ResetInputStream(*input);
    WriteEncodedText(root.get(), std::move(input));
    output_->WriteFooter();
  }

 private:
  std::unordered_map<LetterType, uint32_t> CountLetterFrequencies(
      std::shared_ptr<std::istream> input) {
    auto letter_parser = config_->CreateParser(std::move(input));
    std::unordered_map<LetterType, uint32_t> letter_frequencies;
    while (letter_parser->HasNext()) {
      auto letter = letter_parser->Parse();
      ++letter_frequencies[*letter];
    }
    return letter_frequencies;
  }

  void WriteTreeInPrefixForm(TreeNode* root) {
    if (!root) {
      return;
    }

    WriteNode(root);

    WriteTreeInPrefixForm(root->left_.get());
    WriteTreeInPrefixForm(root->right_.get());
  }

  void WriteNode(TreeNode* node) {
    if (node->isInner()) {
      output_->WriteBit(kInnerNodeBitLabel);
    } else {
      output_->WriteBit(kLeafNodeBitLabel);
      config_->WriteSerialized(*output_, node->key_);
    }
  }

  void WriteEncodedText(TreeNode* root,
                        const std::shared_ptr<std::istream> input) {
    const auto codes_by_letter = BuildCodesMap(root);
    auto letter_parser = config_->CreateParser(std::move(input));
    while (letter_parser->HasNext()) {
      auto letter = letter_parser->Parse();
      if (!letter) {
        break;
      }
      assert(codes_by_letter.contains(*letter));
      for (const auto bit : codes_by_letter.at(*letter)) {
        output_->WriteBit(bit);
      }
    }
  }

  std::shared_ptr<std::istream> input_;
  std::shared_ptr<bit_io::BitWriter> output_;
  std::shared_ptr<Config> config_;
};

template <letter::LetterConfig Config>
class HuffmanDecoder {
 public:
  using LetterType = Config::LetterType;
  using TreeNode = huffman_tree::TreeNode<LetterType>;

  HuffmanDecoder(std::shared_ptr<Config> config,
                 std::shared_ptr<std::istream> input,
                 std::shared_ptr<std::ostream> output)
      : input_(std::make_shared<byte_adapters::ByteAlignedBitReader>(input)),
        output_(std::move(output)),
        config_(std::move(config)) {
    auto root = ReadTreeInPrefixForm();
    WriteDecodedText(root.get());
  }

 private:
  std::unique_ptr<TreeNode> ReadTreeInPrefixForm() {
    const std::optional<bool> bit = input_->ReadBit();
    if (!bit) {
      return nullptr;
    }

    if (*bit == kLeafNodeBitLabel) {
      auto node_key = config_->ReadSerialized(*input_);
      if (!node_key) {
        return nullptr;
      }
      return std::make_unique<TreeNode>(std::move(*node_key), 0, nullptr,
                                        nullptr);
    }

    assert(*bit == kInnerNodeBitLabel);
    auto node = std::make_unique<TreeNode>(LetterType(), 0, nullptr, nullptr);
    node->left_ = std::move(ReadTreeInPrefixForm());
    node->right_ = std::move(ReadTreeInPrefixForm());
    return node;
  }

  void WriteDecodedText(TreeNode* root) {
    if (!root) {
      return;
    }

    auto* current_node = root;
    for (auto bit = input_->ReadBit(); bit; bit = input_->ReadBit()) {
      if (current_node->isInner()) {
        if (*bit == kTurnLeftBitLabel) {
          current_node = current_node->left_.get();
        } else {
          assert(*bit == kTurnRightBitLabel);
          current_node = current_node->right_.get();
        }
      }

      if (current_node->isLeaf()) {
        config_->Write(*output_, current_node->key_);
        current_node = root;
      }
    }
  }

  std::shared_ptr<bit_io::BitReader> input_;
  std::shared_ptr<std::ostream> output_;
  std::shared_ptr<Config> config_;
};

}  // namespace encoding

#endif  // ENCODING_HUFFMAN_ENCODING_H_
