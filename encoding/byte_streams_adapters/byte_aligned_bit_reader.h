#ifndef ENCODING_BYTE_STREAMS_ADAPTERS_BYTE_ISTREAM_ADAPTER_H_
#define ENCODING_BYTE_STREAMS_ADAPTERS_BYTE_ISTREAM_ADAPTER_H_

#include <deque>
#include <istream>
#include <memory>

#include "bit_io/bit_reader.h"

namespace byte_adapters {

class ByteAlignedBitReader : public bit_io::BitReader {
 public:
  explicit ByteAlignedBitReader(
      std::shared_ptr<std::istream> underlying_reader);
  ~ByteAlignedBitReader() override = default;

  std::optional<bool> ReadBit() override;

 private:
  void ConsumeBytes(uint32_t num_bytes);
  void RemoveUnusedBitsInLastByte();
  bool HasAdditionalByteUsed(uint8_t num_unused_bits_in_last_byte);

  std::shared_ptr<std::istream> underlying_reader_;
  std::deque<bool> look_ahead_queue_;
  bool is_last_bit_met_{false};
};

}  // namespace byte_adapters

#endif  // ENCODING_BYTE_STREAMS_ADAPTERS_BYTE_ISTREAM_ADAPTER_H_
