#include "string_io/string_bit_reader.h"

#include "bits_manipulation/bits_manipulation.h"

#include <climits>

namespace string_io {

StringBitReader::StringBitReader(const std::string& data) : data_{data} {}

std::optional<bool> StringBitReader::ReadBit() {
  const uint32_t byte_pos = bit_pos_ / CHAR_BIT;
  if (byte_pos >= data_.size()) {
    return std::nullopt;
  }

  const char byte = data_[byte_pos];
  const bool bit_value =
      bits_manipulation::IsBitEnabled(byte, bit_pos_ % CHAR_BIT);
  ++bit_pos_;
  return bit_value;
}

}  // namespace string_io
