#include "encryption/text_splitter/text_splitter.h"

#include <algorithm>
#include <vector>

namespace huffman_tree {

namespace {
bool StartsWith(std::string_view text, std::string_view pattern) {
  if (pattern.size() > text.size()) {
    return false;
  }

  return text.find(pattern) == 0u;
}
}  // namespace

TextSplitter::TextSplitter(const std::set<std::string>& alphabet)
    : alphabet_{alphabet} {}

std::vector<std::string> TextSplitter::Split(std::string_view text) {
  std::vector<std::string> result;
  std::size_t current_pos = 0;
  while (current_pos < text.size()) {
    std::vector<std::string> feasible_letters;
    for (const auto& letter : alphabet_) {
      if (StartsWith(text.substr(current_pos), letter)) {
        feasible_letters.push_back(letter);
      }
    }

    auto longest_feasible_letter =
        std::max_element(feasible_letters.cbegin(), feasible_letters.cend(),
                         [](const std::string& lhs, const std::string& rhs) {
                           return lhs.size() < rhs.size();
                         });
    current_pos += longest_feasible_letter->size();
    result.push_back(*longest_feasible_letter);
  }
  return result;
}

}  // namespace huffman_tree
