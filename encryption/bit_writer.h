#ifndef ENCRYPTION_BIT_WRITER_H_
#define ENCRYPTION_BIT_WRITER_H_

namespace encryption {

class BitWriter {
 public:
  virtual ~BitWriter() {}

  virtual void WriteBit(bool enabled) = 0;

  virtual void WriteByte(char byte);
};

}  // namespace encryption

#endif  // ENCRYPTION_BIT_WRITER_H_
