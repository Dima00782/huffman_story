#ifndef ENCRYPTION_BYTE_READER_H_
#define ENCRYPTION_BYTE_READER_H_

#include <optional>

namespace encryption {

class ByteReader {
public:
  virtual ~ByteReader() {}

  virtual std::optional<char> ReadByte() = 0;
};

}  // namespace encryption

#endif  // ENCRYPTION_BYTE_READER_H_
