#ifndef ENCRYPTION_HUFFMAN_TREE_BUILDER_H_
#define ENCRYPTION_HUFFMAN_TREE_BUILDER_H_

#include <memory>
#include <string_view>

#include "encryption/byte_reader.h"

namespace encryption {

struct TreeNode {
  TreeNode(const std::string& key, uint32_t frequency,
           std::unique_ptr<TreeNode> left, std::unique_ptr<TreeNode> right);

  std::string key_;
  uint32_t frequency_;
  std::unique_ptr<TreeNode> left_;
  std::unique_ptr<TreeNode> right_;
};

class HuffmanTreeBuilder {
public:
  explicit HuffmanTreeBuilder(std::string_view text);

  std::unique_ptr<TreeNode> GetRoot();

private:
  std::unique_ptr<TreeNode> root_;
};

}  // namespace encryption

#endif  // ENCRYPTION_HUFFMAN_TREE_BUILDER_H_
