#ifndef ENCRYPTION_HUFFMAN_ENCRYPTION_H_
#define ENCRYPTION_HUFFMAN_ENCRYPTION_H_

#include "encryption/bit_reader.h"
#include "encryption/bit_writer.h"
#include "encryption/huffman_tree/huffman_tree_builder.h"

#include <memory>
#include <string_view>

namespace encryption {

class HuffmanEncryption {
 public:
  void Encrypt(std::unique_ptr<BitReader> input,
               std::unique_ptr<BitWriter> output);
  void Decrypt(std::unique_ptr<BitReader> input,
               std::unique_ptr<BitWriter> output);

 private:
  void WriteTreeInPrefixForm(TreeNode* root);
  void WriteEncryptedText(TreeNode* root, std::string_view text);

  std::unique_ptr<BitWriter> output_;
};

}  // namespace encryption

#endif  // ENCRYPTION_HUFFMAN_ENCRYPTION_H_
