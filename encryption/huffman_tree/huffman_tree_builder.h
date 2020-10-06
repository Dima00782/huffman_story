#ifndef ENCRYPTION_HUFFMAN_TREE_HUFFMAN_TREE_BUILDER_H_
#define ENCRYPTION_HUFFMAN_TREE_HUFFMAN_TREE_BUILDER_H_

#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

namespace huffman_tree {

namespace {
template <class InputRange>
std::unordered_map<std::string, uint32_t> CountLetters(const InputRange& text);
}  // namespace

struct TreeNode {
  TreeNode(const std::string& key,
           uint32_t frequency,
           std::unique_ptr<TreeNode> left,
           std::unique_ptr<TreeNode> right)
      : key_{key},
        frequency_{frequency},
        left_{std::move(left)},
        right_{std::move(right)} {}

  std::string key_;
  uint32_t frequency_;
  std::unique_ptr<TreeNode> left_;
  std::unique_ptr<TreeNode> right_;
};

template <class InputRange>
std::unique_ptr<TreeNode> BuildHuffmanTree(InputRange&& text) {
  const auto letter_count = CountLetters(text);

  std::vector<std::unique_ptr<TreeNode>> nodes;
  for (const auto& [letter, frequence] : letter_count) {
    nodes.push_back(
        std::make_unique<TreeNode>(letter, frequence, nullptr, nullptr));
  }

  const auto frequency_comparator = [](const std::unique_ptr<TreeNode>& lhs,
                                       const std::unique_ptr<TreeNode>& rhs) {
    return lhs->frequency_ > rhs->frequency_;
  };

  while (nodes.size() >= 2) {
    std::sort(nodes.begin(), nodes.end(), frequency_comparator);

    auto first_minimum = std::move(nodes.back());
    nodes.pop_back();
    auto second_minimum = std::move(nodes.back());
    nodes.pop_back();

    const auto total_frequency =
        first_minimum->frequency_ + second_minimum->frequency_;
    auto union_node = std::make_unique<TreeNode>("", total_frequency,
                                                 std::move(first_minimum),
                                                 std::move(second_minimum));
    nodes.emplace_back(std::move(union_node));
  }

  if (nodes.empty()) {
    return nullptr;
  }

  return std::move(nodes.back());
}

namespace {
template <class InputRange>
std::unordered_map<std::string, uint32_t> CountLetters(const InputRange& text) {
  std::unordered_map<std::string, uint32_t> letter_count;
  std::for_each(
      std::cbegin(text), std::cend(text),
      [&letter_count](const auto& letter) { ++letter_count[letter]; });
  return letter_count;
}
}  // namespace

}  // namespace huffman_tree

#endif  // ENCRYPTION_HUFFMAN_TREE_HUFFMAN_TREE_BUILDER_H_
