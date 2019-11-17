#ifndef STRING_IO_STRING_BIT_WRITER_H_
#define STRING_IO_STRING_BIT_WRITER_H_

#include <string>
#include <vector>

#include "encryption/bit_writer.h"

namespace string_io {

class StringBitWriter : public encryption::BitWriter {
 public:
  ~StringBitWriter() override = default;

  void WriteBit(bool enabled) override;

  std::string GetData() const;

 private:
  std::vector<bool> bits_;
};

}  // namespace string_io

#endif  // STRING_IO_STRING_BIT_WRITER_H_
