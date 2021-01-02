#ifndef LETTER_LETTER_H_
#define LETTER_LETTER_H_

#include <cstddef>
#include <functional>
#include <istream>
#include <memory>
#include <optional>
#include <string>
#include <vector>

// TODO: extract bit_io from encryption and make it top level package.
#include "encryption/bit_io/bit_reader.h"
#include "encryption/bit_io/bit_writer.h"

namespace letter {

class Letter {
 public:
  virtual ~Letter() {}
  virtual std::size_t Hash() const noexcept = 0;

  // TODO: REMOVE ME.
  virtual std::string toString() const = 0;
};

class LetterSerializer {
 public:
  virtual ~LetterSerializer() {}
  virtual std::unique_ptr<Letter> ReadSerialized(bit_io::BitReader& input) = 0;
  virtual bool WriteSerialized(bit_io::BitWriter& output, const Letter& letter) = 0;
};

class LetterLexer {
 public:
  virtual ~LetterLexer() {}
  virtual std::vector<std::unique_ptr<Letter>> Split(
      std::shared_ptr<std::istream> input) = 0;
};

class LetterConfig {
 public:
  LetterConfig(std::unique_ptr<LetterLexer> lexer,
               std::unique_ptr<LetterSerializer> serializer)
      : lexer_(std::move(lexer)), serializer_(std::move(serializer)) {}

  LetterLexer& Lexer();
  LetterSerializer& Serializer();

 private:
  std::unique_ptr<LetterLexer> lexer_;
  std::unique_ptr<LetterSerializer> serializer_;
};

}  // namespace letter

namespace std {
template <>
struct hash<letter::Letter*> {
  size_t operator()(letter::Letter const* letter) const {
    return letter->Hash();
  }
};
}  // namespace std

#endif  // LETTER_LETTER_H_