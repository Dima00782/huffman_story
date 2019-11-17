#include "encryption/bit_writer.h"

#include <climits>
#include <cstdint>

#include "bits_manipulation/bits_manipulation.h"

namespace encryption {

void BitWriter::WriteByte(char byte) {
  for (uint32_t bit_pos = 0; bit_pos < CHAR_BIT; ++bit_pos) {
    WriteBit(bits_manipulation::IsBitEnabled(byte, bit_pos));
  }
}

}  // namespace encryption