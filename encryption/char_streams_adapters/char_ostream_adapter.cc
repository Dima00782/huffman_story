#include "encryption/char_streams_adapters/char_ostream_adapter.h"

#include <cassert>
#include <climits>
#include <cmath>
#include <cstdint>

#include "bits_manipulation/bits_manipulation.h"

namespace char_adapters {

namespace {
constexpr uint8_t kNumBitsForStoringAlignment =
    static_cast<uint8_t>(std::log2(CHAR_BIT));

constexpr uint32_t kBufferSizeInBits = 1024u * 1024u * 8u;

static_assert(kBufferSizeInBits % CHAR_BIT == 0);
}  // namespace

CharOStreamAdapter::CharOStreamAdapter(std::shared_ptr<std::ostream> ostream)
    : underlying_writer_{ostream} {}

CharOStreamAdapter::~CharOStreamAdapter() {
  if (!has_bits_written_) {
    return;
  }

  WriteFooter();
  FlushBuffer();
}

void CharOStreamAdapter::WriteFooter() {
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

void CharOStreamAdapter::WriteBit(bool enabled) {
  has_bits_written_ = true;
  if (buffer_.size() == kBufferSizeInBits) {
    FlushBuffer();
  }

  buffer_.push_back(enabled);

  num_of_filled_bits_in_last_byte_ =
      (num_of_filled_bits_in_last_byte_ + 1) % CHAR_BIT;
}

void CharOStreamAdapter::FlushBuffer() {
  assert(buffer_.size() % CHAR_BIT == 0);
  for (uint32_t byte_num = 0; byte_num < buffer_.size() / CHAR_BIT;
       ++byte_num) {
    char byte = '\0';
    for (uint32_t bit_pos = 0; bit_pos < CHAR_BIT; ++bit_pos) {
      byte = bits_manipulation::SetBitInByte(
          byte, bit_pos, buffer_[byte_num * CHAR_BIT + bit_pos]);
    }
    underlying_writer_->write(&byte, 1u);
  }
  underlying_writer_->flush();

  buffer_.clear();
}

}  // namespace char_adapters