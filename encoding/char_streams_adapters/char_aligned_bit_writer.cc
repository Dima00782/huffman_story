#include "encoding/char_streams_adapters/char_aligned_bit_writer.h"

#include <cassert>
#include <climits>
#include <cmath>
#include <cstdint>

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
}

bool CharAlignedBitWriter::WriteFooter() {
  assert(!has_footer_was_written_);
  has_footer_was_written_ = true;

  if (!has_any_bits_written_) {
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
  if (!FlushBuffer()) {
    return false;
  }
  return true;
}

bool CharAlignedBitWriter::WriteBit(bool enabled) {
  has_any_bits_written_ = true;
  if (bitset_.SizeInBits() * CHAR_BIT == kBufferSizeInBytes) {
    if (!FlushBuffer()) {
      return false;
    }
  }
  bitset_.PushBack(enabled);
  num_of_filled_bits_in_last_byte_ =
      (num_of_filled_bits_in_last_byte_ + 1) % CHAR_BIT;
  return true;
}

bool CharAlignedBitWriter::FlushBuffer() {
  assert(bitset_.SizeInBits() % CHAR_BIT == 0);
  if (!underlying_writer_->write(bitset_.GetAsCharArray(),
                                 bitset_.SizeInBits() / CHAR_BIT)) {
    return false;
  }
  bitset_.Clear();
  return true;
}

}  // namespace char_adapters