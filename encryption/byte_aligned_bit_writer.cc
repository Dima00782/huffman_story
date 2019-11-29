#include "encryption/byte_aligned_bit_writer.h"

#include <cassert>
#include <climits>
#include <cmath>

namespace encryption {

namespace {
constexpr uint8_t kNumBitsForStoringAlignment =
    static_cast<uint8_t>(std::log2(CHAR_BIT));
}

ByteAlignedBitWriter::ByteAlignedBitWriter(
    std::unique_ptr<BitWriter> underlying_writer)
    : underlying_writer_{std::move(underlying_writer)} {}

ByteAlignedBitWriter::~ByteAlignedBitWriter() {
  if (!has_bit_written_) {
      return;
  }

  auto num_unused_bits_in_last_byte = kNumBitsForStoringAlignment;
  while (num_of_filled_bits_in_last_byte_ !=
         (CHAR_BIT - kNumBitsForStoringAlignment)) {
    WriteBit(false);
    ++num_unused_bits_in_last_byte;
  }

  num_unused_bits_in_last_byte %= CHAR_BIT;
  for (uint8_t bit_pos = 1u; bit_pos <= kNumBitsForStoringAlignment;
       ++bit_pos) {
    const auto bit_value = (num_unused_bits_in_last_byte >>
                            (kNumBitsForStoringAlignment - bit_pos)) &
                           1u;
    WriteBit(bit_value == 1u);
  }

  assert(num_of_filled_bits_in_last_byte_ == 0u);
}

void ByteAlignedBitWriter::WriteBit(bool enabled) {
  has_bit_written_ = true;
  underlying_writer_->WriteBit(enabled);
  num_of_filled_bits_in_last_byte_ =
      (num_of_filled_bits_in_last_byte_ + 1) % CHAR_BIT;
}

}  // namespace encryption