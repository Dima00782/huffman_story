#ifndef STRING_IO_STRING_WRITER_H_
#define STRING_IO_STRING_WRITER_H_

#include <string>

#include "encryption/byte_writer.h"

namespace string_io {

class StringByteWriter : public encryption::ByteWriter {
public:
  ~StringByteWriter() override = default;

  void WriteByte(char byte) override;

  std::string GetData() const;

private:
  std::string data_;
};

}  // namespace string_io

#endif  // STRING_IO_STRING_WRITER_H_
