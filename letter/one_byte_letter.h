#ifndef SYMBOL_ONE_BYTE_SYMBOL_H_
#define SYMBOL_ONE_BYTE_SYMBOL_H_

#include <cstddef>
#include <functional>
#include <istream>
#include <memory>
#include <optional>
#include <string>
#include <utility>

#include "letter/letter.h"

namespace letter {

class OneByteLetter final : public Letter {
 public:
  explicit OneByteLetter(std::byte content) : content_{content} {}
  ~OneByteLetter() override {}

  std::unique_ptr<Letter> ReadSerialized(bit_io::BitReader& input) override {
    const auto byte = input.ReadByte();
    if (byte) {
      return std::make_unique<OneByteLetter>(static_cast<std::byte>(*byte));
    }
    return nullptr;
  }

  void WriteSerialized(bit_io::BitWriter& output) override {
    output.WriteByte(static_cast<char>(content_));
  }

  std::size_t Hash() const noexcept override {
    return std::hash<char>{}(static_cast<char>(content_));
  }

  std::string toString() const override {
    return std::string(1, static_cast<char>(content_));
  }

 private:
  std::byte content_;
};

class OneByteLetterLexer final : public LetterLexer {
 public:
  ~OneByteLetterLexer() override {}

  std::vector<std::unique_ptr<Letter>> Split(std::shared_ptr<std::istream> input) override {
    std::vector<std::unique_ptr<Letter>> letters;
    for (char symbol = '\0'; *input_ >> symbol;) {
      letters.push_back(std::make_unique<OneByteLetter>(static_cast<std::byte>(symbol)));
    }
    return letters;
  }

 private:
  std::shared_ptr<std::istream> input_;
};

}  // namespace symbol

#endif  // SYMBOL_ONE_BYTE_SYMBOL_H_
