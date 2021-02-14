#include "bit_io/bit_writer.h"

#include <climits>
#include <cstdint>

#include "bits_manipulation/bits_manipulation.h"

namespace bit_io {

bool BitWriter::WriteByte(std::byte byte) {
  for (uint32_t bit_pos = 0; bit_pos < CHAR_BIT; ++bit_pos) {
    if (!WriteBit(bits_manipulation::IsBitEnabled(byte, bit_pos))) {
      return false;
    }
  }
  return true;
}

}  // namespace bit_io