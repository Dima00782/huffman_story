#ifndef CHAR_STREAMS_ADAPTERS_CHAR_ISTREAM_ADAPTER_H_
#define CHAR_STREAMS_ADAPTERS_CHAR_ISTREAM_ADAPTER_H_

#include <istream>
#include <memory>
#include <deque>

#include "encryption/bit_reader.h"

namespace char_adapters {

class CharIStreamAdapter : public encryption::BitReader {
 public:
  explicit CharIStreamAdapter(std::shared_ptr<std::istream> underlying_reader);
  ~CharIStreamAdapter() override = default;

  std::optional<bool> ReadBit() override;

 private:
  void RemoveUnusedBitsInLastByte();
  bool HasAdditionalByteUsed(uint8_t num_unused_bits_in_last_byte);

  std::shared_ptr<std::istream> underlying_reader_;
  std::deque<bool> look_ahead_queue_;
  bool is_last_bit_met_{false};
};

}  // namespace char_adapters

#endif  // CHAR_STREAMS_ADAPTERS_CHAR_ISTREAM_ADAPTER_H_
