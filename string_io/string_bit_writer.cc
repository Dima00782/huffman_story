#include "string_io/string_bit_writer.h"

#include <climits>
#include <cstdint>

#include "bits_manipulation/bits_manipulation.h"

namespace string_io {

void StringBitWriter::WriteBit(bool enabled) {
  bits_.push_back(enabled);
}

std::string StringBitWriter::GetData() const {
  const auto num_of_bytes =
      (bits_.size() / CHAR_BIT) + (bits_.size() % CHAR_BIT == 0 ? 0 : 1);
  std::string result_string(num_of_bytes, '\0');

  uint32_t pos = 0;
  for (const auto bit_enabled : bits_) {
    auto& byte = result_string[pos / CHAR_BIT];
    byte = bits_manipulation::SetBitInByte(byte, pos % CHAR_BIT, bit_enabled);
    ++pos;
  }

  return result_string;
}

}  // namespace string_io