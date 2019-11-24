#ifndef STRING_IO_STRING_BIT_READER_H_
#define STRING_IO_STRING_BIT_READER_H_

#include <cstdint>
#include <string>

#include "encryption/bit_reader.h"

namespace string_io {

class StringBitReader : public encryption::BitReader {
 public:
  StringBitReader(const std::string& data);

  ~StringBitReader() override = default;

  std::optional<bool> ReadBit() override;

 private:
  const std::string data_;
  uint32_t bit_pos_{0};
};

}  // namespace string_io

#endif  // STRING_IO_STRING_BIT_READER_H_
