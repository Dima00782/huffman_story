#include "encryption/huffman_tree/huffman_tree_builder.h"

#include <algorithm>
#include <unordered_map>
#include <utility>
#include <vector>

namespace encryption {

namespace {

std::unordered_map<char, uint32_t> CountChars(std::string_view text) {
  std::unordered_map<char, uint32_t> char_count;
  for (const auto byte : text) {
    ++char_count[byte];
  }

  return char_count;
}

}  // namespace

TreeNode::TreeNode(const std::string& key,
                   uint32_t frequency,
                   std::unique_ptr<TreeNode> left,
                   std::unique_ptr<TreeNode> right)
    : key_{key},
      frequency_{frequency},
      left_{std::move(left)},
      right_{std::move(right)} {}

HuffmanTreeBuilder::HuffmanTreeBuilder(std::string_view text) {
  const auto char_count = CountChars(text);

  std::vector<std::unique_ptr<TreeNode>> nodes;
  for (const auto& [symbol, frequence] : char_count) {
    nodes.push_back(std::make_unique<TreeNode>(std::string(1, symbol),
                                               frequence, nullptr, nullptr));
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

std::unique_ptr<TreeNode> HuffmanTreeBuilder::GetRoot() {
  return std::move(root_);
}

}  // namespace encryption
