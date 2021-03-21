#ifndef LETTER_FIXED_ALPHABET_LETTER_TRIE_H_
#define LETTER_FIXED_ALPHABET_LETTER_TRIE_H_

#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>

namespace fixed_alpha_letter {
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
}  // namespace fixed_alpha_letter

#endif  // LETTER_FIXED_ALPHABET_LETTER_TRIE_H_
