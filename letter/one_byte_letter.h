#ifndef SYMBOL_ONE_BYTE_SYMBOL_H_
#define SYMBOL_ONE_BYTE_SYMBOL_H_

#include <cassert>
#include <cstddef>
#include <functional>
#include <istream>
#include <memory>
#include <optional>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

#include "bit_io/bit_reader.h"
#include "bit_io/bit_writer.h"

namespace letter {

// TODO: unify interface
// Split -> vector<letter> and Write(ostream&, std::vector<letters>)
// WriteSerialized & ReadSerialized

class ByteLetterLexer final {
 public:
  std::vector<std::byte> Split(std::istream& input) {
    std::vector<std::byte> letters;
    // TODO: toooo slow;
    for (char letter = '\0'; input.get(letter);) {
      letters.push_back(static_cast<std::byte>(letter));
    }
    return letters;
  }
};

class ByteLetterSerializer final {
 public:
  std::optional<std::byte> ReadSerialized(bit_io::BitReader& input) {
    const auto byte = input.ReadByte();
    if (!byte) {
      return std::nullopt;
    }
    return *byte;
  }

  bool WriteSerialized(bit_io::BitWriter& output, const std::byte letter) {
    output.WriteByte(letter);
    // TODO: FIXME.
    return true;
  }

  bool Write(std::ostream& output, const std::byte letter) {
    output.put(static_cast<char>(letter));
    // TODO: FIXME.
    return true;
  }
};

class OneByteLetterConfig {
 public:
  using LetterType = std::byte;

  std::vector<LetterType> Parse(std::istream& input) {
    return lexer_.Split(input);
  }

  bool Write(std::ostream& output, LetterType letter) {
    return serializer_.Write(output, letter);
  }

  bool WriteSerialized(bit_io::BitWriter& bit_output, LetterType letter) {
    return serializer_.WriteSerialized(bit_output, letter);
  }

  std::optional<LetterType> ReadSerialized(bit_io::BitReader& bit_reader) {
    return serializer_.ReadSerialized(bit_reader);
  }

 private:
  ByteLetterLexer lexer_;
  ByteLetterSerializer serializer_;
};

}  // namespace letter

#endif  // SYMBOL_ONE_BYTE_SYMBOL_H_
