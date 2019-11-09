#include "string_io/string_byte_writer.h"

namespace string_io {

void StringByteWriter::WriteByte(char byte) {
  data_.push_back(byte);
}

std::string StringByteWriter::GetData() const {
  return data_;
}

}  // namespace string_io