#ifndef ENCRYPTION_HUFFMAN_ENCRYPTION_H_
#define ENCRYPTION_HUFFMAN_ENCRYPTION_H_

#include "encryption/bit_reader.h"
#include "encryption/bit_writer.h"
#include "encryption/huffman_tree/huffman_tree_builder.h"

#include <memory>
#include <optional>
#include <deque>
#include <string_view>

namespace encryption {

class HuffmanEncryption {
 public:
  void Encrypt(std::unique_ptr<BitReader> input,
               std::unique_ptr<BitWriter> output);
  void Decrypt(std::unique_ptr<BitReader> input,
               std::unique_ptr<BitWriter> output);

 private:
  std::unique_ptr<TreeNode> ReadTreeInPrefixForm();
  void WriteTreeInPrefixForm(TreeNode* root);

  void WriteDecryptedText(TreeNode* root);
  void WriteEncryptedText(TreeNode* root, std::string_view text);

  void WriteBit(bool enabled);
  void WriteByte(char byte);
  void WriteAlignment();

  std::optional<bool> ReadBit();
  std::optional<char> ReadByte();

  void PopulateQueue();
  void RemoveUnusedBitsInLastByte();

  std::deque<bool> look_ahead_queue_;
  bool is_last_bit_met_{false};

  uint8_t alignment_{0u};
  std::unique_ptr<BitReader> input_;
  std::unique_ptr<BitWriter> output_;
};

}  // namespace encryption

#endif  // ENCRYPTION_HUFFMAN_ENCRYPTION_H_
