#ifndef LETTER_TEXT_SPLITTER_TRIE_H_
#define LETTER_TEXT_SPLITTER_TRIE_H_

#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>

// TODO: should be in unnamed namespace
namespace text_splitter {
class Trie {
 public:
  struct Node {
    std::unordered_map<char, std::unique_ptr<Node>> childs_;
    bool is_complete_;

    Node* GetChildBySymbol(const char ch) {
      auto it = childs_.find(ch);
      if (it == childs_.end()) {
        return nullptr;
      }

      return it->second.get();
    }
  };

  Trie();

  void Add(std::string_view word);
  std::string GetLongestMatchingWord(std::string_view text);
  std::size_t MaxWordLenght() const { return max_word_lenght_; }

 private:
  std::size_t max_word_lenght_;
  std::unique_ptr<Node> root_;
};
}  // namespace text_splitter

#endif  // LETTER_TEXT_SPLITTER_TEXT_SPLITTER_H_
