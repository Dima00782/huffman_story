#ifndef ENCRYPTION_HUFFMAN_TREE_HUFFMAN_TREE_BUILDER_H_
#define ENCRYPTION_HUFFMAN_TREE_HUFFMAN_TREE_BUILDER_H_

#include <memory>
#include <set>
#include <string>
#include <string_view>
#include <vector>

namespace huffman_tree {

struct TreeNode {
  TreeNode(const std::string& key,
           uint32_t frequency,
           std::unique_ptr<TreeNode> left,
           std::unique_ptr<TreeNode> right);

  std::string key_;
  uint32_t frequency_;
  std::unique_ptr<TreeNode> left_;
  std::unique_ptr<TreeNode> right_;
};

class HuffmanTreeBuilder {
 public:
  explicit HuffmanTreeBuilder(const std::vector<std::string>& text);

  std::unique_ptr<TreeNode> GetRoot();

 private:
  std::unique_ptr<TreeNode> root_;
};

}  // namespace huffman_tree

#endif  // ENCRYPTION_HUFFMAN_TREE_HUFFMAN_TREE_BUILDER_H_
