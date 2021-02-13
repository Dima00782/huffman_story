#ifndef BIT_IO_BIT_READER_H_
#define BIT_IO_BIT_READER_H_

#include <cstddef>
#include <optional>

namespace bit_io {

class BitReader {
 public:
  virtual ~BitReader() {}

  virtual std::optional<bool> ReadBit() = 0;
  virtual std::optional<std::byte> ReadByte();
};

}  // namespace bit_io

#endif  // BIT_IO_BIT_READER_H_
