#include "encryption/huffman_encryption.h"

#include <cassert>
#include <iterator>
#include <limits>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "encryption/bit_io/bit_reader.h"
#include "encryption/bit_io/bit_writer.h"
#include "encryption/char_streams_adapters/char_aligned_bit_reader.h"
#include "encryption/char_streams_adapters/char_aligned_bit_writer.h"
#include "encryption/huffman_tree/huffman_tree_builder.h"
#include "encryption/text_splitter/text_splitter.h"

namespace encryption {

namespace {

constexpr bool kInnerNodeBitLabel = false;
constexpr bool kLeafNodeBitLabel = true;
constexpr bool kTurnLeftBitLabel = false;
constexpr bool kTurnRightBitLabel = true;
constexpr uint8_t kNumBitsForKeySize = 8u;

std::unordered_map<std::shared_ptr<letter::Letter>, std::vector<bool>>
BuildCodesMap(huffman_tree::TreeNode* root);

}  // namespace

HuffmanEncrypt::HuffmanEncrypt(
    std::shared_ptr<std::istream> input,
    std::shared_ptr<std::ostream> output,
    std::shared_ptr<letter::LetterLexer> lexer,
    std::shared_ptr<letter::LetterSerializer> serializer)
    : output_{std::make_shared<char_adapters::CharAlignedBitWriter>(
          std::move(output))},
      serializer_{std::move(serializer)} {
  const auto splittedText = std::move(lexer)->Split(std::move(input));
  auto root = huffman_tree::BuildHuffmanTree(splittedText);
  WriteTreeInPrefixForm(root.get());
  WriteEncryptedText(root.get(), splittedText);
}

void HuffmanEncrypt::WriteTreeInPrefixForm(huffman_tree::TreeNode* root) {
  if (!root) {
    return;
  }

  WriteNode(root);

  WriteTreeInPrefixForm(root->left_.get());
  WriteTreeInPrefixForm(root->right_.get());
}

void HuffmanEncrypt::WriteNode(huffman_tree::TreeNode* node) {
  if (node->isInner()) {
    output_->WriteBit(kInnerNodeBitLabel);
  } else {
    output_->WriteBit(kLeafNodeBitLabel);
    serializer_->WriteSerialized(*output_, *node->key_);
  }
}

void HuffmanEncrypt::WriteEncryptedText(
    huffman_tree::TreeNode* root,
    const std::vector<std::shared_ptr<letter::Letter>>& text) {
  const auto codes_by_letter = BuildCodesMap(root);
  for (const auto& letter : text) {
    assert(codes_by_letter.contains(letter));
    for (const auto bit : codes_by_letter.at(letter)) {
      output_->WriteBit(bit);
    }
  }
}

namespace {
std::unordered_map<std::shared_ptr<letter::Letter>, std::vector<bool>>
BuildCodesMap(huffman_tree::TreeNode* root) {
  assert(root);
  std::unordered_map<std::shared_ptr<letter::Letter>, std::vector<bool>> codes;

  if (root->isLeaf()) {
    codes[root->key_] = std::vector<bool>(1, kTurnLeftBitLabel);
    return codes;
  }

  struct NodeWithCode {
    huffman_tree::TreeNode* node;
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
}  // namespace

HuffmanDecrypt::HuffmanDecrypt(
    std::shared_ptr<std::istream> input,
    std::shared_ptr<std::ostream> output,
    std::shared_ptr<letter::LetterSerializer> serializer)
    : input_{std::make_shared<char_adapters::CharAlignedBitReader>(input)},
      serializer_{std::move(serializer)} {
  output_ = std::move(output);
  auto root = ReadTreeInPrefixForm();
  WriteDecryptedText(root.get());
}

std::unique_ptr<huffman_tree::TreeNode> HuffmanDecrypt::ReadTreeInPrefixForm() {
  // TODO: remove auto, make it std::optional<bool> and then use if (!bit)
  const auto bit = input_->ReadBit();
  if (!bit.has_value()) {
    return nullptr;
  }

  if (*bit == kLeafNodeBitLabel) {
    auto node_key = serializer_->ReadSerialized(*input_);
    return std::make_unique<huffman_tree::TreeNode>(std::move(node_key), 0,
                                                    nullptr, nullptr);
  }

  assert(*bit == kInnerNodeBitLabel);
  auto node =
      std::make_unique<huffman_tree::TreeNode>(nullptr, 0, nullptr, nullptr);
  node->left_ = std::move(ReadTreeInPrefixForm());
  node->right_ = std::move(ReadTreeInPrefixForm());
  return node;
}

void HuffmanDecrypt::WriteDecryptedText(huffman_tree::TreeNode* root) {
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
      // TODO: FIXME!
      output_->put(current_node->key_->toString().back());
      current_node = root;
    }
  }
}

}  // namespace encryption
