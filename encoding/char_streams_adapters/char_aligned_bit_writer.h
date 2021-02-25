#ifndef ENCODING_CHAR_STREAMS_ADAPTERS_CHAR_OSTREAM_ADAPTER_H_
#define ENCODING_CHAR_STREAMS_ADAPTERS_CHAR_OSTREAM_ADAPTER_H_

#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <ostream>

#include "bit_io/bit_writer.h"

namespace {
constexpr uint32_t kBufferSizeInBytes = 1024u * 1024u;
}

namespace char_adapters {

class CharAlignedBitWriter final : public bit_io::BitWriter {
 public:
  explicit CharAlignedBitWriter(std::shared_ptr<std::ostream> ostream);
  ~CharAlignedBitWriter() override;

  bool WriteBit(bool enabled) override;
  bool WriteFooter() override;

 private:
  void FlushBuffer();

  // TODO: can be replaced with fixed size bitset that can converts into char* array.
  std::array<std::byte, kBufferSizeInBytes> buffer_;
  uint32_t bit_idx_{0u};
  bool has_bits_written_{false};
  uint8_t num_of_filled_bits_in_last_byte_{0u};
  std::shared_ptr<std::ostream> underlying_writer_;
  bool has_footer_was_written_{false};
};

}  // namespace char_adapters

#endif  // ENCODING_CHAR_STREAMS_ADAPTERS_CHAR_OSTREAM_ADAPTER_H_
