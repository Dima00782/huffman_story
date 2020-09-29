#ifndef ENCRYPTION_HUFFMAN_ENCRYPTION_H_
#define ENCRYPTION_HUFFMAN_ENCRYPTION_H_

#include <istream>
#include <memory>
#include <ostream>
#include <set>
#include <string>
#include <vector>

namespace huffman_tree {
struct TreeNode;
}

namespace bit_io {
class BitWriter;
class BitReader;
}  // namespace bit_io

namespace encryption {

class HuffmanEncrypt {
 public:
  HuffmanEncrypt(std::shared_ptr<std::istream> input,
                 std::shared_ptr<std::ostream> output,
                 const std::set<std::string>& alphabet);

 private:
  void WriteTreeInPrefixForm(huffman_tree::TreeNode* root);
  void WriteNode(huffman_tree::TreeNode* node);
  void WriteNodeKey(const std::string& key);
  void WriteKeySize(const std::size_t size);

  void WriteEncryptedText(huffman_tree::TreeNode* root,
                          const std::vector<std::string>& text);

  std::shared_ptr<std::istream> input_;
  std::shared_ptr<bit_io::BitWriter> output_;
};

class HuffmanDecrypt {
 public:
  HuffmanDecrypt(std::shared_ptr<std::istream> input,
                 std::shared_ptr<std::ostream> output);

 private:
  std::unique_ptr<huffman_tree::TreeNode> ReadTreeInPrefixForm();
  void WriteDecryptedText(huffman_tree::TreeNode* root);
  std::string ReadNodeKey();

  std::shared_ptr<bit_io::BitReader> input_;
  std::shared_ptr<std::ostream> output_;
};

}  // namespace encryption

#endif  // ENCRYPTION_HUFFMAN_ENCRYPTION_H_
