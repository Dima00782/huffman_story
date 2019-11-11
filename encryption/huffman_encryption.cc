#include "encryption/huffman_encryption.h"

#include <string_view>
#include <utility>

namespace encryption {

namespace {

std::string ReadBytesToString(std::unique_ptr<ByteReader> byte_reader) {
  std::string result;
  for (auto byte = byte_reader->ReadByte(); byte.has_value(); byte = byte_reader->ReadByte()) {
    result.push_back(*byte);
  }

  return result;
}

}  // namespace

HuffmanEncryption::HuffmanEncryption(std::unique_ptr<ByteReader> byte_reader,
                                     std::unique_ptr<ByteWriter> byte_writer)
  : byte_writer_{std::move(byte_writer)} {
  const auto text = ReadBytesToString(std::move(byte_reader));
  auto root = HuffmanTreeBuilder(text).GetRoot();
  Encrypt(std::move(root), text);
}

void HuffmanEncryption::Encrypt(std::unique_ptr<TreeNode> root, std::string_view text) {
  WriteTree(root.get());
  WriteEncryptedText(root.get(), text);
}

void HuffmanEncryption::WriteTree(TreeNode* root) {
  
}

void HuffmanEncryption::WriteEncryptedText(TreeNode* root, std::string_view text) {
  
}

}  // namespace encryption
