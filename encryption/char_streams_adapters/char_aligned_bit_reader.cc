#include "encryption/char_streams_adapters/char_aligned_bit_reader.h"

#include <cassert>
#include <climits>
#include <cmath>
#include <utility>

#include "bits_manipulation/bits_manipulation.h"

namespace char_adapters {

namespace {
constexpr uint8_t kNumBitsForStoringAlignment =
    static_cast<uint8_t>(std::log2(CHAR_BIT));
constexpr uint32_t kMinimumQueueSizeInBytes = 1024u;
}  // namespace

CharAlignedBitReader::CharAlignedBitReader(
    std::shared_ptr<std::istream> underlying_reader)
    : underlying_reader_{underlying_reader} {
  ConsumeBytes(kMinimumQueueSizeInBytes);
}

void CharAlignedBitReader::ConsumeBytes(uint32_t num_bytes) {
  for (uint32_t i = 0u; i < num_bytes; ++i) {
    char byte = '\0';
    if (underlying_reader_->get(byte)) {
      for (uint32_t bit_pos = 0; bit_pos < CHAR_BIT; ++bit_pos) {
        const auto bit = bits_manipulation::IsBitEnabled(byte, bit_pos);
        look_ahead_queue_.push_back(bit);
      }
    } else if (!is_last_bit_met_) {
      is_last_bit_met_ = true;
      RemoveUnusedBitsInLastByte();
    }
  }
}

std::optional<bool> CharAlignedBitReader::ReadBit() {
  if (look_ahead_queue_.empty()) {
    return std::nullopt;
  }

  const bool bit_value = look_ahead_queue_.front();
  look_ahead_queue_.pop_front();

  if (look_ahead_queue_.empty()) {
    ConsumeBytes(kMinimumQueueSizeInBytes);
  }

  return bit_value;
}

void CharAlignedBitReader::RemoveUnusedBitsInLastByte() {
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

bool CharAlignedBitReader::HasAdditionalByteUsed(
    uint8_t num_unused_bits_in_last_byte) {
  return num_unused_bits_in_last_byte < kNumBitsForStoringAlignment;
}

}  // namespace char_adapters