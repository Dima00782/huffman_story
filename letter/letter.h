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
  virtual std::shared_ptr<Letter> ReadSerialized(bit_io::BitReader& input) = 0;
  virtual bool WriteSerialized(bit_io::BitWriter& output,
                               const Letter& letter) = 0;
};

class LetterLexer {
 public:
  virtual ~LetterLexer() {}
  virtual std::vector<std::shared_ptr<Letter>> Split(
      std::shared_ptr<std::istream> input) = 0;
};

}  // namespace letter

namespace std {
template <>
struct hash<letter::Letter*> {
  size_t operator()(letter::Letter const* letter) const {
    return letter->Hash();
  }
};

template <>
struct equal_to<unique_ptr<letter::Letter>> {
  bool operator()(const unique_ptr<letter::Letter>& lhs, const unique_ptr<letter::Letter>& rhs) const {
    // Will be ugly with dynamic casts for checking if two different Letters are actually the same type.
    return lhs->toString() == rhs->toString();
  }
};

template <>
struct equal_to<shared_ptr<letter::Letter>> {
  bool operator()(const shared_ptr<letter::Letter>& lhs, const shared_ptr<letter::Letter>& rhs) const {
    // Will be ugly with dynamic casts for checking if two different Letters are actually the same type.
    return lhs->toString() == rhs->toString();
  }
};

}  // namespace std

#endif  // LETTER_LETTER_H_
