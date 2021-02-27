#include "encoding/char_streams_adapters/char_aligned_bit_writer.h"

#include <cassert>
#include <climits>
#include <cmath>
#include <cstdint>

#include "bits_manipulation/bits_manipulation.h"

namespace char_adapters {

namespace {
constexpr uint8_t kNumBitsForStoringAlignment =
    static_cast<uint8_t>(std::log2(CHAR_BIT));
}

CharAlignedBitWriter::CharAlignedBitWriter(
    std::shared_ptr<std::ostream> ostream)
    : underlying_writer_{ostream} {}

CharAlignedBitWriter::~CharAlignedBitWriter() {
  assert(has_footer_was_written_);
  FlushBuffer();
}

bool CharAlignedBitWriter::WriteFooter() {
  assert(!has_footer_was_written_);
  has_footer_was_written_ = true;

  if (!has_bits_written_) {
    return true;
  }

  auto num_unused_bits_in_last_byte = kNumBitsForStoringAlignment;
  while (num_of_filled_bits_in_last_byte_ !=
         (CHAR_BIT - kNumBitsForStoringAlignment)) {
    if (!WriteBit(false)) {
      return false;
    }
    ++num_unused_bits_in_last_byte;
  }

  num_unused_bits_in_last_byte %= CHAR_BIT;
  for (uint8_t bit_pos = 1u; bit_pos <= kNumBitsForStoringAlignment;
       ++bit_pos) {
    const auto bit_value = (num_unused_bits_in_last_byte >>
                            (kNumBitsForStoringAlignment - bit_pos)) &
                           1u;
    if (!WriteBit(bit_value == 1u)) {
      return false;
    }
  }

  assert(num_of_filled_bits_in_last_byte_ == 0u);
  FlushBuffer();
  return true;
}

bool CharAlignedBitWriter::WriteBit(bool enabled) {
  has_bits_written_ = true;
  if (bit_idx_ * CHAR_BIT == kBufferSizeInBytes) {
    FlushBuffer();
  }

  std::byte& byte = buffer_[bit_idx_ / CHAR_BIT];
  byte = bits_manipulation::SetBitInByte(byte, bit_idx_ % CHAR_BIT, enabled);
  ++bit_idx_;

  num_of_filled_bits_in_last_byte_ =
      (num_of_filled_bits_in_last_byte_ + 1) % CHAR_BIT;

  return true;
}

void CharAlignedBitWriter::FlushBuffer() {
  assert(bit_idx_ % CHAR_BIT == 0);
  underlying_writer_->write(reinterpret_cast<char*>(&buffer_[0]),
                            bit_idx_ / CHAR_BIT);
  bit_idx_ = 0u;
}

}  // namespace char_adapters