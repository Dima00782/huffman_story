#ifndef STRING_IO_STRING_BIT_WRITER_H_
#define STRING_IO_STRING_BIT_WRITER_H_

#include <string>
#include <vector>

#include "encryption/bit_writer.h"

namespace string_io {

class StringBitWriter : public encryption::BitWriter {
 public:
  explicit StringBitWriter(std::string* buffer);
  ~StringBitWriter() override;

  void WriteBit(bool enabled) override;

 private:
  std::string* buffer_;
  std::vector<bool> bits_;
};

}  // namespace string_io

#endif  // STRING_IO_STRING_BIT_WRITER_H_
