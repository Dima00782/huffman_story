#include "letter/fixed_alphabet_letter/trie.h"

#include <algorithm>

namespace letter {

Trie::Trie() : root_{std::make_unique<Node>(Node{{}, false})} {}

void Trie::Add(std::string_view word) {
  max_word_lenght_ = std::max(word.size(), max_word_lenght_);
  Node* current = root_.get();

  for (const auto symbol : word) {
    Node* next = current->GetChildBySymbol(symbol);
    if (!next) {
      auto new_child = std::make_unique<Node>(Node{{}, false});
      next = new_child.get();
      current->childs_.insert({symbol, std::move(new_child)});
    }
    current = next;
  }

  current->is_complete_ = true;
}

std::string Trie::GetLongestMatchingWord(std::string_view text) {
  Node* current = root_.get();
  std::string longest_word;
  std::string current_word;

  for (const auto symbol : text) {
    current = current->GetChildBySymbol(symbol);
    if (!current) {
      break;
    }
    current_word += symbol;
    if (current->is_complete_) {
      longest_word = current_word;
    }
  }

  return longest_word;
}

}  // namespace letter
