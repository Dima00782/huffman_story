#include "encryption/huffman_encryption.h"

#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <cassert>

namespace encryption {

namespace {

std::unordered_map<char, std::string> BuildCodesMap(TreeNode* root);
bool IsInnerNode(encryption::TreeNode* node);
std::string ReadBytesToString(std::unique_ptr<ByteReader> byte_reader);

}  // namespace

HuffmanEncryption::HuffmanEncryption(std::unique_ptr<ByteReader> byte_reader,
                                     std::unique_ptr<ByteWriter> byte_writer)
  : byte_writer_{std::move(byte_writer)} {
  const auto text = ReadBytesToString(std::move(byte_reader));
  auto root = HuffmanTreeBuilder(text).GetRoot();
  Encrypt(std::move(root), text);
}

namespace {
std::string ReadBytesToString(std::unique_ptr<ByteReader> byte_reader) {
  std::string result;
  for (auto byte = byte_reader->ReadByte(); byte.has_value(); byte = byte_reader->ReadByte()) {
    result.push_back(*byte);
  }

  return result;
}
}  // namespace

void HuffmanEncryption::Encrypt(std::unique_ptr<TreeNode> root, std::string_view text) {
  WriteTree(root.get());
  WriteEncryptedText(root.get(), text);
}

void HuffmanEncryption::WriteTree(TreeNode* root) {
  constexpr char kInnerNodeLabel = '0';
  constexpr char kLeafNodeLabel = '1';

  if (!root) {
    return;
  }

  const char node_label = IsInnerNode(root) ? kInnerNodeLabel : kLeafNodeLabel;
  byte_writer_->WriteByte(node_label);
  if (node_label == kLeafNodeLabel) {
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

void HuffmanEncryption::WriteEncryptedText(TreeNode* root, std::string_view text) {
  const auto codes_by_symbol = BuildCodesMap(root);

  for (const auto symbol : text) {
    auto code_iter = codes_by_symbol.find(symbol);
    assert(code_iter != codes_by_symbol.cend());
    for (const auto& byte : code_iter->second) {
      byte_writer_->WriteByte(byte);
    }
  }
}

namespace {
std::unordered_map<char, std::string> BuildCodesMap(TreeNode* root) {
  constexpr char kLeftTurnLabel = '0';
  constexpr char kRightTurnLabel = '1';
  std::unordered_map<char, std::string> codes;

  struct NodeWithCode {
    TreeNode* node;
    std::string code;
  };

  std::unordered_set<const TreeNode*> used;
  std::stack<NodeWithCode> stack;

  stack.push(NodeWithCode{root, root->key_});

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

    if (current_node->left_ && used.find(current_node->left_.get()) == used.end()) {
      stack.push(NodeWithCode{current_node->left_.get(), current.code + kLeftTurnLabel});
    }

    if (current_node->right_ && used.find(current_node->right_.get()) == used.end()) {
      stack.push(NodeWithCode{current_node->right_.get(), current.code + kRightTurnLabel});
    }

    used.insert(current_node);
  }

  return codes;
}
}  // namespace

}  // namespace encryption
