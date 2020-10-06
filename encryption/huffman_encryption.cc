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
#include "encryption/char_streams_adapters/char_istream_adapter.h"
#include "encryption/char_streams_adapters/char_ostream_adapter.h"
#include "encryption/huffman_tree/huffman_tree_builder.h"
#include "encryption/text_splitter/text_splitter.h"

namespace encryption {

namespace {

constexpr bool kInnerNodeBitLabel = false;
constexpr bool kLeafNodeBitLabel = true;
constexpr bool kTurnLeftBitLabel = false;
constexpr bool kTurnRightBitLabel = true;
constexpr uint8_t kNumBitsForKeySize = 8u;

std::unordered_map<std::string, std::vector<bool>> BuildCodesMap(
    huffman_tree::TreeNode* root);

std::string ReadStreamToString(std::istream& input);

bool IsInnerNode(huffman_tree::TreeNode* node) {
  return node->left_ || node->right_;
}

bool IsLeafNode(huffman_tree::TreeNode* node) {
  return !IsInnerNode(node);
}

}  // namespace

HuffmanEncrypt::HuffmanEncrypt(std::shared_ptr<std::istream> input,
                               std::shared_ptr<std::ostream> output,
                               const std::set<std::string>& alphabet)
    : output_{std::make_shared<char_adapters::CharOStreamAdapter>(output)} {
  const std::string text = ReadStreamToString(*input);
  const auto splitted_text = text_splitter::TextSplitter(alphabet).Split(text);
  auto root = huffman_tree::BuildHuffmanTree(splitted_text);

  WriteTreeInPrefixForm(root.get());
  WriteEncryptedText(root.get(), splitted_text);
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
  if (IsInnerNode(node)) {
    output_->WriteBit(kInnerNodeBitLabel);
  } else {
    output_->WriteBit(kLeafNodeBitLabel);
    WriteNodeKey(node->key_);
  }
}

void HuffmanEncrypt::WriteNodeKey(const std::string& key) {
  WriteKeySize(key.size());

  for (const auto byte : key) {
    output_->WriteByte(byte);
  }
}

void HuffmanEncrypt::WriteKeySize(const std::size_t size) {
  assert(size < std::numeric_limits<unsigned char>::max());
  const auto key_size = static_cast<char>(size);
  output_->WriteByte(key_size);
}

void HuffmanEncrypt::WriteEncryptedText(huffman_tree::TreeNode* root,
                                        const std::vector<std::string>& text) {
  const auto codes_by_symbol = BuildCodesMap(root);

  for (const auto symbol : text) {
    const auto code_iter = codes_by_symbol.find(symbol);
    assert(code_iter != codes_by_symbol.end());
    for (const auto bit : code_iter->second) {
      output_->WriteBit(bit);
    }
  }
}

namespace {
std::string ReadStreamToString(std::istream& input) {
  std::string text{(std::istreambuf_iterator<char>(input)),
                   std::istreambuf_iterator<char>()};
  return text;
}

std::unordered_map<std::string, std::vector<bool>> BuildCodesMap(
    huffman_tree::TreeNode* root) {
  std::unordered_map<std::string, std::vector<bool>> codes;

  if (root && !root->left_ && !root->right_) {
    codes[root->key_] = std::vector<bool>(1, kTurnLeftBitLabel);
    return codes;
  }

  struct NodeWithCode {
    huffman_tree::TreeNode* node;
    std::vector<bool> code;
  };

  std::unordered_set<const huffman_tree::TreeNode*> used;
  std::stack<NodeWithCode> stack;

  stack.push(NodeWithCode{root, std::vector<bool>()});

  while (!stack.empty()) {
    const auto current = stack.top();
    const auto* current_node = current.node;
    stack.pop();

    if (!current_node) {
      continue;
    }

    if (!current_node->left_ && !current_node->right_) {
      codes[current_node->key_] = current.code;
      continue;
    }

    if (current_node->left_ &&
        used.find(current_node->left_.get()) == used.end()) {
      auto code = current.code;
      code.push_back(kTurnLeftBitLabel);
      stack.push(NodeWithCode{current_node->left_.get(), std::move(code)});
    }

    if (current_node->right_ &&
        used.find(current_node->right_.get()) == used.end()) {
      auto code = current.code;
      code.push_back(kTurnRightBitLabel);
      stack.push(NodeWithCode{current_node->right_.get(), std::move(code)});
    }

    used.insert(current_node);
  }

  return codes;
}
}  // namespace

HuffmanDecrypt::HuffmanDecrypt(std::shared_ptr<std::istream> input,
                               std::shared_ptr<std::ostream> output)
    : input_{std::make_shared<char_adapters::CharIStreamAdapter>(input)} {
  output_ = std::move(output);
  auto root = ReadTreeInPrefixForm();
  WriteDecryptedText(root.get());
}

std::unique_ptr<huffman_tree::TreeNode> HuffmanDecrypt::ReadTreeInPrefixForm() {
  const auto bit = input_->ReadBit();
  if (!bit.has_value()) {
    return nullptr;
  }

  if (*bit == kLeafNodeBitLabel) {
    const auto node_key = ReadNodeKey();
    return std::make_unique<huffman_tree::TreeNode>(node_key, 0, nullptr,
                                                    nullptr);
  }

  assert(*bit == kInnerNodeBitLabel);
  auto node = std::make_unique<huffman_tree::TreeNode>("", 0, nullptr, nullptr);
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
    if (!IsLeafNode(current_node)) {
      if (*bit == kTurnLeftBitLabel) {
        current_node = current_node->left_.get();
      } else {
        assert(*bit == kTurnRightBitLabel);
        current_node = current_node->right_.get();
      }
    }

    if (IsLeafNode(current_node)) {
      output_->put(current_node->key_.back());
      current_node = root;
    }
  }
}

std::string HuffmanDecrypt::ReadNodeKey() {
  const auto size_byte = input_->ReadByte();
  assert(size_byte.has_value());

  std::string node_key;
  for (uint8_t i = 0; i < size_byte; ++i) {
    const auto symbol = input_->ReadByte();
    assert(symbol.has_value());
    node_key += *symbol;
  }

  return node_key;
}

}  // namespace encryption
