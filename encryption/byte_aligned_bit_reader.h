#ifndef ENCRYPTION_BYTE_ALIGNED_BIT_READER_H_
#define ENCRYPTION_BYTE_ALIGNED_BIT_READER_H_

#include <deque>
#include <memory>

#include "encryption/bit_reader.h"

namespace encryption {

class ByteAlignedBitReader : public BitReader {
 public:
  explicit ByteAlignedBitReader(std::unique_ptr<BitReader> underlying_reader);
  ~ByteAlignedBitReader() override = default;

  std::optional<bool> ReadBit() override;

 private:
  void RemoveUnusedBitsInLastByte();

  std::unique_ptr<BitReader> underlying_reader_;
  std::deque<bool> look_ahead_queue_;
  bool is_last_bit_met_{false};
};

}  // namespace encryption

#endif  // ENCRYPTION_BYTE_ALIGNED_BIT_READER_H_
