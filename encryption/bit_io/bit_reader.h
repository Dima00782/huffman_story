#ifndef ENCRYPTION_BIT_IO_BIT_READER_H_
#define ENCRYPTION_BIT_IO_BIT_READER_H_

#include <optional>
#include <cstddef>

namespace bit_io {

class BitReader {
 public:
  virtual ~BitReader() {}

  virtual std::optional<bool> ReadBit() = 0;
  virtual std::optional<std::byte> ReadByte();
};

}  // namespace bit_io

#endif  // ENCRYPTION_BIT_IO_BIT_READER_H_
