#ifndef ENCRYPTION_HUFFMAN_TREE_TEXT_SPLITTER_H_
#define ENCRYPTION_HUFFMAN_TREE_TEXT_SPLITTER_H_

#include <set>
#include <string>
#include <string_view>
#include <vector>

namespace huffman_tree {

class TextSplitter {
 public:
  explicit TextSplitter(const std::set<std::string>& alphabet);

  std::vector<std::string> Split(std::string_view text);

 private:
  const std::set<std::string> alphabet_;
};

}  // namespace huffman_tree

#endif  // ENCRYPTION_HUFFMAN_TREE_TEXT_SPLITTER_H_
