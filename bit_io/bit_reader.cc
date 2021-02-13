#include "bit_io/bit_reader.h"
#include "bits_manipulation/bits_manipulation.h"

#include <climits>
#include <cstdint>

namespace bit_io {

std::optional<std::byte> BitReader::ReadByte() {
  std::byte byte{0};
  for (uint8_t bit_pos = 0u; bit_pos < CHAR_BIT; ++bit_pos) {
    const auto bit_value = ReadBit();
    if (!bit_value.has_value()) {
      return std::nullopt;
    }
    byte = bits_manipulation::SetBitInByte(byte, bit_pos, *bit_value);
  }
  
  return byte;
}

}  // namespace bit_io