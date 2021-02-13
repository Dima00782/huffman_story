#include "bits_manipulation/bits_manipulation.h"

namespace bits_manipulation {

bool IsBitEnabled(std::byte byte, uint8_t pos) {
  return (byte & (std::byte{1} << (CHAR_BIT - pos - 1))) != std::byte{0};
}

std::byte SetBitInByte(std::byte byte, uint8_t pos, bool enabled) {
  if (enabled) {
    return byte | (std::byte{1} << (CHAR_BIT - pos - 1));
  }

  return byte & ~(std::byte{1} << (CHAR_BIT - pos - 1));
}

std::array<bool, CHAR_BIT> ByteToBits(std::byte byte) {
  std::array<bool, CHAR_BIT> bits;
  for (uint32_t bit_pos = 0; bit_pos < bits.size(); ++bit_pos) {
    bits[bit_pos] = IsBitEnabled(byte, bit_pos);
  }

  return bits;
}

}  // namespace bits_manipulation
