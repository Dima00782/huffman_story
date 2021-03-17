#ifndef ENCODING_BYTE_STREAMS_ADAPTERS_BYTE_OSTREAM_ADAPTER_H_
#define ENCODING_BYTE_STREAMS_ADAPTERS_BYTE_OSTREAM_ADAPTER_H_

#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <ostream>

#include "bit_io/bit_writer.h"
#include "bits_manipulation/bits_manipulation.h"

namespace {
constexpr uint32_t kBufferSizeInBytes = 1024u * 1024u;
}

namespace byte_adapters {

class ByteAlignedBitWriter final : public bit_io::BitWriter {
 public:
  explicit ByteAlignedBitWriter(std::shared_ptr<std::ostream> ostream);
  ~ByteAlignedBitWriter() override;

  bool WriteBit(bool enabled) override;
  bool WriteFooter() override;

 private:
  bool FlushBuffer();

  bits_manipulation::FixedSizeBitSet<kBufferSizeInBytes> bitset_;
  uint8_t num_of_filled_bits_in_last_byte_{0u};
  std::shared_ptr<std::ostream> underlying_writer_;
  bool has_any_bits_written_{false};
  bool has_footer_was_written_{false};
};

}  // namespace byte_adapters

#endif  // ENCODING_BYTE_STREAMS_ADAPTERS_BYTE_OSTREAM_ADAPTER_H_
