#ifndef ENCRYPTION_BIT_READER_H_
#define ENCRYPTION_BIT_READER_H_

#include <optional>

namespace encryption {

class BitReader {
 public:
  virtual ~BitReader() {}

  virtual std::optional<bool> ReadBit() = 0;
  virtual std::optional<char> ReadByte();
};

}  // namespace encryption

#endif  // ENCRYPTION_BIT_READER_H_
