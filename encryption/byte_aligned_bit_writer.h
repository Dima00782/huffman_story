#ifndef ENCRYPTION_BYTE_ALIGNED_BIT_WRITER_H_
#define ENCRYPTION_BYTE_ALIGNED_BIT_WRITER_H_

#include <memory>

#include "encryption/bit_writer.h"

namespace encryption {

class ByteAlignedBitWriter : public BitWriter {
 public:
  explicit ByteAlignedBitWriter(std::unique_ptr<BitWriter> underlying_writer);
  ~ByteAlignedBitWriter() override;

  void WriteBit(bool enabled) override;

 private:
  bool has_bit_written_{false};
  uint8_t num_of_filled_bits_in_last_byte_{0u};
  std::unique_ptr<BitWriter> underlying_writer_;
};

}  // namespace encryption

#endif  // ENCRYPTION_BYTE_ALIGNED_BIT_READER_H_
