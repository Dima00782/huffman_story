#ifndef ENCRYPTION_HUFFMAN_TREE_HUFFMAN_TREE_BUILDER_H_
#define ENCRYPTION_HUFFMAN_TREE_HUFFMAN_TREE_BUILDER_H_

#include <algorithm>
#include <cassert>
#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

namespace huffman_tree {

namespace {
template <class LetterType, class InputRange>
std::unordered_map<LetterType, uint32_t> CountLetters(const InputRange& text);
}  // namespace

template <class LetterType>
struct TreeNode {
  TreeNode(LetterType key,
           uint32_t frequency,
           std::unique_ptr<TreeNode<LetterType>> left,
           std::unique_ptr<TreeNode<LetterType>> right)
      : key_(std::move(key)),
        frequency_(frequency),
        left_(std::move(left)),
        right_(std::move(right)) {}

  bool isLeaf() const { return !left_ && !right_; }
  bool isInner() const { return left_ && right_; }

  LetterType key_;
  uint32_t frequency_;
  std::unique_ptr<TreeNode<LetterType>> left_;
  std::unique_ptr<TreeNode<LetterType>> right_;
};

template <class LetterType, class InputRange>
std::unique_ptr<TreeNode<LetterType>> BuildHuffmanTree(const InputRange& text) {
  const auto letter_count = CountLetters<LetterType, InputRange>(text);

  if (letter_count.empty()) {
    return nullptr;
  }

  std::vector<std::unique_ptr<TreeNode<LetterType>>> nodes;
  for (const auto& [letter, frequence] : letter_count) {
    nodes.push_back(
        std::make_unique<TreeNode<LetterType>>(letter, frequence, nullptr, nullptr));
  }

  const auto frequency_comparator = [](const std::unique_ptr<TreeNode<LetterType>>& lhs,
                                       const std::unique_ptr<TreeNode<LetterType>>& rhs) {
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
    auto union_node = std::make_unique<TreeNode<LetterType>>(LetterType(), total_frequency,
                                                 std::move(first_minimum),
                                                 std::move(second_minimum));
    nodes.emplace_back(std::move(union_node));
  }

  assert(nodes.size() == 1);
  return std::move(nodes.back());
}

namespace {
template <class LetterType, class InputRange>
std::unordered_map<LetterType, uint32_t> CountLetters(const InputRange& text) {
  std::unordered_map<LetterType, uint32_t> letter_count;
  std::for_each(std::cbegin(text), std::cend(text),
                [&letter_count](auto& letter) { ++letter_count[letter]; });
  return letter_count;
}
}  // namespace

}  // namespace huffman_tree

#endif  // ENCRYPTION_HUFFMAN_TREE_HUFFMAN_TREE_BUILDER_H_
