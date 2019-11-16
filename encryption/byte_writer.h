#ifndef ENCRYPTION_BYTE_WRITER_H_
#define ENCRYPTION_BYTE_WRITER_H_

namespace encryption {

class ByteWriter {
 public:
  virtual ~ByteWriter() {}

  virtual void WriteByte(char byte) = 0;
};

}  // namespace encryption

#endif  // ENCRYPTION_BYTE_WRITER_H_
