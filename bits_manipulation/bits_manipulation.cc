#include "bits_manipulation/bits_manipulation.h"

namespace bits_manipulation {

bool IsBitEnabled(char byte, uint8_t pos) {
  return (byte & (1 << (CHAR_BIT - pos - 1))) != 0;
}

char SetBitInByte(char byte, uint8_t pos, bool enabled) {
  if (enabled) {
    return byte | (1 << (CHAR_BIT - pos - 1));
  }

  return byte & ~(1 << (CHAR_BIT - pos - 1));
}

std::array<bool, CHAR_BIT> ByteToBits(char byte) {
  std::array<bool, CHAR_BIT> bits;
  for (uint32_t bit_pos = 0; bit_pos < bits.size(); ++bit_pos) {
    bits[bit_pos] = IsBitEnabled(byte, bit_pos);
  }

  return bits;
}

}  // namespace bits_manipulation
