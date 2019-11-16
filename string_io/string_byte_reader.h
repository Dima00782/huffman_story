#ifndef STRING_IO_STRING_READER_H_
#define STRING_IO_STRING_READER_H_

#include <string>

#include "encryption/byte_reader.h"

namespace string_io {

class StringByteReader : public encryption::ByteReader {
 public:
  StringByteReader(const std::string& data);

  ~StringByteReader() override = default;

  std::optional<char> ReadByte() override;

 private:
  const std::string data_;
  std::string::const_iterator pos_;
};

}  // namespace string_io

#endif  // STRING_IO_STRING_READER_H_
