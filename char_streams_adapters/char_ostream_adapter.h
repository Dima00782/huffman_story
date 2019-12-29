#ifndef CHAR_STREAMS_ADAPTERS_CHAR_OSTREAM_ADAPTER_H_
#define CHAR_STREAMS_ADAPTERS_CHAR_OSTREAM_ADAPTER_H_

#include <memory>
#include <ostream>
#include <vector>

#include "encryption/bit_writer.h"

namespace char_adapters {

class CharOStreamAdapter : public encryption::BitWriter {
 public:
  explicit CharOStreamAdapter(std::shared_ptr<std::ostream> ostream);
  ~CharOStreamAdapter() override;

  void WriteBit(bool enabled) override;

 private:
  void FlushBuffer();
  void WriteFooter();

  std::vector<bool> buffer_;
  bool has_bits_written_{false};
  uint8_t num_of_filled_bits_in_last_byte_{0u};
  std::shared_ptr<std::ostream> underlying_writer_;
};

}  // namespace char_adapters

#endif  // CHAR_STREAMS_ADAPTERS_CHAR_OSTREAM_ADAPTER_H_
