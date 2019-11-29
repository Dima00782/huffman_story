#ifndef ENCRYPTION_HUFFMAN_ENCRYPTION_H_
#define ENCRYPTION_HUFFMAN_ENCRYPTION_H_

#include <memory>
#include <optional>
#include <string_view>

namespace encryption {

class BitWriter;
class BitReader;
class ByteAlignedBitReader;
struct TreeNode;

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

  std::unique_ptr<ByteAlignedBitReader> input_;
  uint8_t alignment_{0u};
  std::unique_ptr<BitWriter> output_;
};

}  // namespace encryption

#endif  // ENCRYPTION_HUFFMAN_ENCRYPTION_H_
