#include "encoding/text_splitter/text_splitter.h"

#include <algorithm>
#include <string_view>
#include <vector>

namespace text_splitter {

TextSplitter::TextSplitter(const std::set<std::string>& alphabet) {
  for (const auto& letter : alphabet) {
    trie_.Add(letter);
  }
}

std::vector<std::string> TextSplitter::Split(std::string_view text) {
  std::vector<std::string> result;
  while (!text.empty()) {
    auto longest_letter = trie_.GetLongestMatchingWord(text);
    text = text.substr(longest_letter.size());
    result.push_back(std::move(longest_letter));
  }
  return result;
}

}  // namespace text_splitter
