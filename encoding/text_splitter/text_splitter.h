#ifndef ENCODING_TEXT_SPLITTER_TEXT_SPLITTER_H_
#define ENCODING_TEXT_SPLITTER_TEXT_SPLITTER_H_

#include <set>
#include <string>
#include <string_view>
#include <vector>

#include "encoding/text_splitter/trie.h"

namespace text_splitter {

class TextSplitter {
 public:
  explicit TextSplitter(const std::set<std::string>& alphabet);

  std::vector<std::string> Split(std::string_view text);

 private:
  Trie trie_;
};

}  // namespace text_splitter

#endif  // ENCODING_TEXT_SPLITTER_TEXT_SPLITTER_H_
