#include "encryption/byte_aligned_bit_reader.h"

#include <cassert>
#include <climits>
#include <cmath>
#include <utility>

#include "bits_manipulation/bits_manipulation.h"

namespace encryption {

namespace {
constexpr uint8_t kNumBitsForStoringAlignment =
    static_cast<uint8_t>(std::log2(CHAR_BIT));

constexpr uint32_t kMinimumQueueSize = 16u;
}  // namespace

ByteAlignedBitReader::ByteAlignedBitReader(
    std::unique_ptr<BitReader> underlying_reader)
    : underlying_reader_{std::move(underlying_reader)} {
  for (uint32_t i = 0u; i < kMinimumQueueSize; ++i) {
    const auto bit = underlying_reader_->ReadBit();
    if (!bit.has_value()) {
      return;
    }
    look_ahead_queue_.push_back(*bit);
  }
}

std::optional<bool> ByteAlignedBitReader::ReadBit() {
  if (look_ahead_queue_.empty()) {
    return std::nullopt;
  }

  const bool bit_value = look_ahead_queue_.front();
  look_ahead_queue_.pop_front();

  const auto bit = underlying_reader_->ReadBit();
  if (bit) {
    look_ahead_queue_.push_back(*bit);
  } else if (!is_last_bit_met_) {
    is_last_bit_met_ = true;
    RemoveUnusedBitsInLastByte();
  }

  return bit_value;
}

void ByteAlignedBitReader::RemoveUnusedBitsInLastByte() {
  char num_unused_bits_in_last_byte = '\0';
  for (uint8_t bit_pos = 1; bit_pos <= kNumBitsForStoringAlignment; ++bit_pos) {
    const bool bit_enabled = look_ahead_queue_.back();
    const uint8_t pos_from_end = CHAR_BIT - bit_pos;
    num_unused_bits_in_last_byte = bits_manipulation::SetBitInByte(
        num_unused_bits_in_last_byte, pos_from_end, bit_enabled);
    look_ahead_queue_.pop_back();
  }

  if (HasAdditionalByteUsed(num_unused_bits_in_last_byte)) {
    num_unused_bits_in_last_byte += CHAR_BIT;
  }

  num_unused_bits_in_last_byte -= kNumBitsForStoringAlignment;
  for (uint8_t unused_bit_id = 0; unused_bit_id < num_unused_bits_in_last_byte;
       ++unused_bit_id) {
    look_ahead_queue_.pop_back();
  }
}

bool ByteAlignedBitReader::HasAdditionalByteUsed(
    uint8_t num_unused_bits_in_last_byte) {
  return num_unused_bits_in_last_byte < kNumBitsForStoringAlignment;
}

}  // namespace encryption
