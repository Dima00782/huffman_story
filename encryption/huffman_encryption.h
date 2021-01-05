#ifndef ENCRYPTION_HUFFMAN_ENCRYPTION_H_
#define ENCRYPTION_HUFFMAN_ENCRYPTION_H_

#include <istream>
#include <memory>
#include <ostream>
#include <set>
#include <string>
#include <vector>

#include "letter/letter.h"

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
                 std::shared_ptr<letter::LetterLexer> extractor,
                 std::shared_ptr<letter::LetterSerializer> serializer);

 private:
  void WriteTreeInPrefixForm(huffman_tree::TreeNode* root);
  void WriteNode(huffman_tree::TreeNode* node);

  void WriteEncryptedText(huffman_tree::TreeNode* root,
                          const std::vector<std::shared_ptr<letter::Letter>>& text);

  std::shared_ptr<std::istream> input_;
  std::shared_ptr<bit_io::BitWriter> output_;
  std::shared_ptr<letter::LetterSerializer> serializer_;
};

class HuffmanDecrypt {
 public:
  HuffmanDecrypt(std::shared_ptr<std::istream> input,
                 std::shared_ptr<std::ostream> output,
                 std::shared_ptr<letter::LetterSerializer> serializer);

 private:
  std::unique_ptr<huffman_tree::TreeNode> ReadTreeInPrefixForm();
  void WriteDecryptedText(huffman_tree::TreeNode* root);

  std::shared_ptr<bit_io::BitReader> input_;
  std::shared_ptr<std::ostream> output_;
  std::shared_ptr<letter::LetterSerializer> serializer_;
};

}  // namespace encryption

#endif  // ENCRYPTION_HUFFMAN_ENCRYPTION_H_
