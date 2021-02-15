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

struct OneByteLetterConfig {
  using LetterType = std::byte;
  using LetterLexerType = ByteLetterLexer;
  using LetterSerializerType = ByteLetterSerializer;
};

}  // namespace letter

#endif  // SYMBOL_ONE_BYTE_SYMBOL_H_
