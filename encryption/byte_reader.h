#ifndef ENCRYPTION_BYTE_READER_H_
#define ENCRYPTION_BYTE_READER_H_

#include <cstddef>

class ByteReader {
public:
  virtual ~ByteReader() {}

  virtual std::byte ReadByte();
};

#endif  // ENCRYPTION_BYTE_READER_H_