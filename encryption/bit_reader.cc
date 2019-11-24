#include "encryption/bit_reader.h"
#include "bits_manipulation/bits_manipulation.h"

#include <climits>
#include <cstdint>

namespace encryption {

std::optional<char> BitReader::ReadByte() {
  char byte = '\0';
  for (uint8_t bit_pos = 0u; bit_pos < CHAR_BIT; ++bit_pos) {
    const auto bit_value = ReadBit();
    if (!bit_value.has_value()) {
      return std::nullopt;
    }
    byte = bits_manipulation::SetBitInByte(byte, bit_pos, *bit_value);
  }
  
  return byte;
}

}  // namespace encryption