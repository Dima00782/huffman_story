#ifndef ENCRYPTION_HUFFMAN_ENCRYPTION_H_
#define ENCRYPTION_HUFFMAN_ENCRYPTION_H_

#include <memory>
#include <optional>
#include <string_view>

namespace encryption {

class BitWriter;
class BitReader;
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

  std::unique_ptr<BitReader> input_;
  std::unique_ptr<BitWriter> output_;
};

}  // namespace encryption

#endif  // ENCRYPTION_HUFFMAN_ENCRYPTION_H_
