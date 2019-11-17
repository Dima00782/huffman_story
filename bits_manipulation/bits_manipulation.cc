#include "bits_manipulation/bits_manipulation.h"

#include <climits>

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

std::vector<bool> ByteToBits(char byte) {
  std::vector<bool> bits(CHAR_BIT, false);
  for (uint32_t bit_pos = 0; bit_pos < CHAR_BIT; ++bit_pos) {
    bits[bit_pos] = IsBitEnabled(byte, bit_pos);
  }

  return bits;
}

std::string InBinaryForm(uint16_t number) {
  return std::string{static_cast<char>(number >> CHAR_BIT),
                     static_cast<char>(number)};
}

}  // namespace bits_manipulation
