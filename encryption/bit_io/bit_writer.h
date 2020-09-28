#ifndef ENCRYPTION_BIT_IO_BIT_WRITER_H_
#define ENCRYPTION_BIT_IO_BIT_WRITER_H_

namespace bit_io {

class BitWriter {
 public:
  virtual ~BitWriter() {}

  virtual void WriteBit(bool enabled) = 0;

  virtual void WriteByte(char byte);
};

}  // namespace bit_io

#endif  // ENCRYPTION_BIT_IO_BIT_WRITER_H_
