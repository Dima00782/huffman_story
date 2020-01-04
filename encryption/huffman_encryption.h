#ifndef ENCRYPTION_HUFFMAN_ENCRYPTION_H_
#define ENCRYPTION_HUFFMAN_ENCRYPTION_H_

#include <istream>
#include <memory>
#include <optional>
#include <ostream>
#include <string_view>
#include <set>
#include <string>

namespace huffman_tree {
struct TreeNode;
}

namespace encryption {

class BitWriter;
class BitReader;

class HuffmanEncrypt {
 public:
  HuffmanEncrypt(std::shared_ptr<std::istream> input,
                 std::shared_ptr<BitWriter> output,
                 const std::set<std::string>& alphabet);

 private:
  void WriteTreeInPrefixForm(huffman_tree::TreeNode* root);
  void WriteEncryptedText(huffman_tree::TreeNode* root, std::string_view text);

  std::shared_ptr<std::istream> input_;
  std::shared_ptr<BitWriter> output_;
  std::set<std::string> alphabet_;
};

class HuffmanDecrypt {
 public:
  HuffmanDecrypt(std::shared_ptr<BitReader> input,
                 std::shared_ptr<std::ostream> output);

 private:
  std::unique_ptr<huffman_tree::TreeNode> ReadTreeInPrefixForm();
  void WriteDecryptedText(huffman_tree::TreeNode* root);

  std::shared_ptr<BitReader> input_;
  std::shared_ptr<std::ostream> output_;
};

}  // namespace encryption

#endif  // ENCRYPTION_HUFFMAN_ENCRYPTION_H_
