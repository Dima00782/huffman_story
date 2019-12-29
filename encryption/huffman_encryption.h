#ifndef ENCRYPTION_HUFFMAN_ENCRYPTION_H_
#define ENCRYPTION_HUFFMAN_ENCRYPTION_H_

#include <istream>
#include <memory>
#include <optional>
#include <ostream>
#include <string_view>

namespace encryption {

class BitWriter;
class BitReader;
struct TreeNode;

class HuffmanEncrypt {
 public:
  HuffmanEncrypt(std::shared_ptr<std::istream> input,
                 std::shared_ptr<BitWriter> output);

 private:
  void WriteTreeInPrefixForm(TreeNode* root);
  void WriteEncryptedText(TreeNode* root, std::string_view text);

  std::shared_ptr<std::istream> input_;
  std::shared_ptr<BitWriter> output_;
};

class HuffmanDecrypt {
 public:
  HuffmanDecrypt(std::shared_ptr<BitReader> input,
                 std::shared_ptr<std::ostream> output);

 private:
  std::unique_ptr<TreeNode> ReadTreeInPrefixForm();
  void WriteDecryptedText(TreeNode* root);

  std::shared_ptr<BitReader> input_;
  std::shared_ptr<std::ostream> output_;
};

}  // namespace encryption

#endif  // ENCRYPTION_HUFFMAN_ENCRYPTION_H_
