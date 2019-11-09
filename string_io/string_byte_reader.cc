#include "string_io/string_byte_reader.h"

namespace string_io {

StringByteReader::StringByteReader(const std::string& data)
  : data_{data},
    pos_{data_.cbegin()}
{}

std::optional<char> StringByteReader::ReadByte() {
  if (pos_ == data_.cend()) {
    return std::nullopt;
  }

  const auto result = *pos_;
  ++pos_;
  return result;
}

}  // namespace string_io
