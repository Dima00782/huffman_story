#ifndef SYMBOL_ONE_BYTE_SYMBOL_H_
#define SYMBOL_ONE_BYTE_SYMBOL_H_

#include <cassert>
#include <cstddef>
#include <functional>
#include <istream>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "bit_io/bit_reader.h"
#include "bit_io/bit_writer.h"

namespace letter {

class ByteLetterLexer final {
 public:
  ~ByteLetterLexer() {}

  std::vector<std::byte> Split(std::shared_ptr<std::istream> input) {
    std::vector<std::byte> letters;
    for (char letter = '\0'; input->get(letter);) {
      letters.push_back(static_cast<std::byte>(letter));
    }
    return letters;
  }
};

class ByteLetterSerializer final {
 public:
  ~ByteLetterSerializer() {}

  std::optional<std::byte> ReadSerialized(bit_io::BitReader& input) {
    // TODO: why we return optional<char>, can it be optional<std::byte>?
    const auto byte = input.ReadByte();
    if (!byte) {
      return std::nullopt;
    }
    return static_cast<std::byte>(*byte);
  }

  bool WriteSerialized(bit_io::BitWriter& output, const std::byte letter) {
    output.WriteByte(letter);
    // TODO: FIXME.
    return true;
  }
};

}  // namespace letter

#endif  // SYMBOL_ONE_BYTE_SYMBOL_H_
