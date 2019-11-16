#ifndef ENCRYPTION_HUFFMAN_ENCRYPTION_H_
#define ENCRYPTION_HUFFMAN_ENCRYPTION_H_

#include "encryption/byte_reader.h"
#include "encryption/byte_writer.h"
#include "encryption/huffman_tree/huffman_tree_builder.h"

#include <memory>
#include <string_view>

namespace encryption {

class HuffmanEncryption {
 public:
  HuffmanEncryption(std::unique_ptr<ByteReader> byte_reader,
                    std::unique_ptr<ByteWriter> byte_writer);

 private:
  void Encrypt(std::unique_ptr<TreeNode> root, std::string_view text);
  void WriteTree(TreeNode* root);
  void WriteEncryptedText(TreeNode* root, std::string_view text);

  std::unique_ptr<ByteWriter> byte_writer_;
};

}  // namespace encryption

#endif  // ENCRYPTION_HUFFMAN_ENCRYPTION_H_
