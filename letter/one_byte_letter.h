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

class ByteLetterParser final {
 public:
  explicit ByteLetterParser(std::shared_ptr<std::istream> input)
      : input_(std::move(input)) {}

  std::optional<std::byte> Parse() {
    char byte = '\0';
    if (input_->get(byte)) {
      return static_cast<std::byte>(byte);
    }
    return std::nullopt;
  }

  bool HasNext() const { return input_->rdbuf()->in_avail() > 0; }

 private:
  std::shared_ptr<std::istream> input_;
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
    if (!output.WriteByte(letter)) {
      return false;
    }
    return true;
  }

  bool Write(std::ostream& output, const std::byte letter) {
    if (!output.put(static_cast<char>(letter))) {
      return false;
    }
    return true;
  }
};

class OneByteLetterConfig {
 public:
  using LetterType = std::byte;
  using LetterParser = ByteLetterParser;

  std::unique_ptr<LetterParser> CreateParser(
      std::shared_ptr<std::istream> input) {
    return std::make_unique<LetterParser>(std::move(input));
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
  ByteLetterSerializer serializer_;
};

}  // namespace letter

#endif  // SYMBOL_ONE_BYTE_SYMBOL_H_
