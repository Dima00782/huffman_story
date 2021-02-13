#ifndef BIT_IO_BIT_WRITER_H_
#define BIT_IO_BIT_WRITER_H_

#include <cstddef>

namespace bit_io {

class BitWriter {
 public:
  virtual ~BitWriter() {}

  virtual void WriteBit(bool enabled) = 0;

  virtual void WriteByte(std::byte byte);
};

}  // namespace bit_io

#endif  // BIT_IO_BIT_WRITER_H_
