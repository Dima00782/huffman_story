#include "encryption/huffman_tree/huffman_tree_builder.h"

#include <algorithm>
#include <climits>
#include <cstdint>
#include <unordered_map>
#include <utility>
#include <vector>

#include "bits_manipulation/bits_manipulation.h"

namespace huffman_tree {

namespace {
std::unordered_map<std::string, uint32_t> CountLetters(
    std::vector<std::string> text);
}  // namespace

TreeNode::TreeNode(const std::string& key,
                   uint32_t frequency,
                   std::unique_ptr<TreeNode> left,
                   std::unique_ptr<TreeNode> right)
    : key_{key},
      frequency_{frequency},
      left_{std::move(left)},
      right_{std::move(right)} {}

HuffmanTreeBuilder::HuffmanTreeBuilder(const std::vector<std::string>& text) {
  if (text.empty()) {
    return;
  }

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

  root_ = std::move(nodes.back());
}

namespace {
std::unordered_map<std::string, uint32_t> CountLetters(
    std::vector<std::string> text) {
  std::unordered_map<std::string, uint32_t> letter_count;
  for (const auto letter : text) {
    ++letter_count[letter];
  }

  return letter_count;
}
}  // namespace

std::unique_ptr<TreeNode> HuffmanTreeBuilder::GetRoot() {
  return std::move(root_);
}

}  // namespace huffman_tree
