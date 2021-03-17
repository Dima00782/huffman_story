#include "encoding/byte_streams_adapters/byte_aligned_bit_reader.h"

#include <cassert>
#include <climits>
#include <cmath>
#include <utility>

#include "bits_manipulation/bits_manipulation.h"

namespace byte_adapters {

namespace {
constexpr uint8_t kNumBitsForStoringAlignment =
    static_cast<uint8_t>(std::log2(CHAR_BIT));
constexpr uint32_t kMinimumQueueSizeInBytes = 1024u;
}  // namespace

ByteAlignedBitReader::ByteAlignedBitReader(
    std::shared_ptr<std::istream> underlying_reader)
    : underlying_reader_{underlying_reader} {
  ConsumeBytes(kMinimumQueueSizeInBytes);
}

void ByteAlignedBitReader::ConsumeBytes(uint32_t num_bytes) {
  for (uint32_t i = 0u; i < num_bytes; ++i) {
    char byte = '\0';
    if (underlying_reader_->get(byte)) {
      for (uint32_t bit_pos = 0; bit_pos < CHAR_BIT; ++bit_pos) {
        const auto bit = bits_manipulation::IsBitEnabled(
            static_cast<std::byte>(byte), bit_pos);
        look_ahead_queue_.push_back(bit);
      }
    } else if (!is_last_bit_met_) {
      is_last_bit_met_ = true;
      RemoveUnusedBitsInLastByte();
    }
  }
}

std::optional<bool> ByteAlignedBitReader::ReadBit() {
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

void ByteAlignedBitReader::RemoveUnusedBitsInLastByte() {
  uint8_t num_unused_bits_in_last_byte = 0u;
  for (uint8_t bit_pos = 1; bit_pos <= kNumBitsForStoringAlignment; ++bit_pos) {
    const bool bit_enabled = look_ahead_queue_.back();
    const uint8_t pos_from_end = CHAR_BIT - bit_pos;
    num_unused_bits_in_last_byte = std::to_integer<uint8_t>(
        bits_manipulation::SetBitInByte(std::byte{num_unused_bits_in_last_byte},
                                        pos_from_end, bit_enabled));
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

}  // namespace byte_adapters