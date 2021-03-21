#ifndef LETTER_FIXED_ALPHABET_LETTER_H_
#define LETTER_FIXED_ALPHABET_LETTER_H_

#include <cassert>
#include <istream>
#include <memory>
#include <set>
#include <string>

#include "bit_io/bit_reader.h"
#include "bit_io/bit_writer.h"
#include "letter/fixed_alphabet_letter/trie.h"

namespace letter {

class FixedAlphabetLetterParser final {
 public:
  explicit FixedAlphabetLetterParser(std::set<std::string> alphabet,
                                     std::shared_ptr<std::istream> input)
      : input_(std::move(input)) {
    for (const auto& letter : alphabet) {
      trie_.Add(letter);
    }
    // TODO: add all characters too.
  }

  std::optional<std::string> Parse() { return std::nullopt; }

  bool HasNext() const { return input_->rdbuf()->in_avail() > 0; }

 private:
  Trie trie_;
  std::shared_ptr<std::istream> input_;
};

class FixedAlphabetLetterConfig {
 public:
  using LetterType = std::string;
  using LetterParser = FixedAlphabetLetterParser;

  FixedAlphabetLetterConfig(std::set<std::string> alphabet)
      : alphabet_(std::move(alphabet)) {}

  std::unique_ptr<LetterParser> CreateParser(
      std::shared_ptr<std::istream> input) {
    return std::make_unique<LetterParser>(alphabet_, std::move(input));
  }

  bool Write(std::ostream& output, LetterType letter) {
    // TODO: implement me.
    return false;
  }

  bool WriteSerialized(bit_io::BitWriter& bit_output, LetterType letter) {
    // TODO: implement me.
    return false;
  }

  std::optional<LetterType> ReadSerialized(bit_io::BitReader& bit_reader) {
    // TODO: implement me.
    return std::nullopt;
  }

 private:
  std::set<std::string> alphabet_;
};

}  // namespace letter

#endif  // LETTER_FIXED_ALPHABET_LETTER_H_