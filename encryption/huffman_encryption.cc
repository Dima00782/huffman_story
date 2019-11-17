#include "encryption/huffman_encryption.h"

#include <cassert>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "bits_manipulation/bits_manipulation.h"

namespace encryption {

namespace {

std::unordered_map<char, std::vector<bool>> BuildCodesMap(TreeNode* root);
bool IsInnerNode(encryption::TreeNode* node);
std::string ReadBytesToString(std::unique_ptr<ByteReader> byte_reader);

}  // namespace

HuffmanEncryption::HuffmanEncryption(std::unique_ptr<ByteReader> byte_reader,
                                     std::unique_ptr<BitWriter> byte_writer)
    : byte_writer_{std::move(byte_writer)} {
  const auto text = ReadBytesToString(std::move(byte_reader));
  auto root = HuffmanTreeBuilder(text).GetRoot();
  Encrypt(std::move(root), text);
}

namespace {
std::string ReadBytesToString(std::unique_ptr<ByteReader> byte_reader) {
  std::string result;
  for (auto byte = byte_reader->ReadByte(); byte.has_value();
       byte = byte_reader->ReadByte()) {
    result.push_back(*byte);
  }

  return result;
}
}  // namespace

void HuffmanEncryption::Encrypt(std::unique_ptr<TreeNode> root,
                                std::string_view text) {
  WriteTree(root.get());
  WriteEncryptedText(root.get(), text);
}

void HuffmanEncryption::WriteTree(TreeNode* root) {
  constexpr bool kInnerNodeBitLabel = false;
  constexpr bool kLeafNodeBitLabel = true;

  if (!root) {
    return;
  }

  const auto node_label =
      IsInnerNode(root) ? kInnerNodeBitLabel : kLeafNodeBitLabel;
  byte_writer_->WriteBit(node_label);
  if (node_label == kLeafNodeBitLabel) {
    byte_writer_->WriteByte(root->key_.back());
  }

  WriteTree(root->left_.get());
  WriteTree(root->right_.get());
}

namespace {
bool IsInnerNode(encryption::TreeNode* node) {
  return node->left_ && node->right_;
}
}  // namespace

void HuffmanEncryption::WriteEncryptedText(TreeNode* root,
                                           std::string_view text) {
  const auto codes_by_symbol = BuildCodesMap(root);

  for (const auto symbol : text) {
    const auto code_iter = codes_by_symbol.find(symbol);
    assert(code_iter != codes_by_symbol.end());
    for (const auto bit : code_iter->second) {
      byte_writer_->WriteBit(bit);
    }
  }
}

namespace {
std::unordered_map<char, std::vector<bool>> BuildCodesMap(TreeNode* root) {
  constexpr bool kTurnLeftBitLabel = false;
  constexpr bool kTurnRightBitLabel = true;
  std::unordered_map<char, std::vector<bool>> codes;

  if (!root) {
    return codes;
  }

  if (!root->left_ && !root->right_) {
    codes[root->key_.back()] = std::vector<bool>(1, kTurnLeftBitLabel);
    return codes;
  }

  struct NodeWithCode {
    TreeNode* node;
    std::vector<bool> code;
  };

  std::unordered_set<const TreeNode*> used;
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
      codes[current_node->key_.back()] = current.code;
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

}  // namespace encryption
