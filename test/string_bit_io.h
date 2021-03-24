#ifndef TEST_STRING_BIT_IO_H_
#define TEST_STRING_BIT_IO_H_

#include <cstdint>
#include <optional>
#include <string>

#include "bits_manipulation/bits_manipulation.h"

namespace test {

class StringBasedBitReader : public bit_io::BitReader {
 public:
  explicit StringBasedBitReader(const std::string& content)
      : content_(content) {}
  ~StringBasedBitReader() override = default;

  std::optional<bool> ReadBit() override {
    const uint32_t byte_index = bit_index_ / CHAR_BIT;
    if (byte_index >= content_.size()) {
      return std::nullopt;
    }

    const bool result = bits_manipulation::IsBitEnabled(
        static_cast<std::byte>(content_[byte_index]), bit_index_ % CHAR_BIT);
    ++bit_index_;
    return result;
  }

 private:
  uint32_t bit_index_{0};
  std::string content_;
};

class StringBasedBitWriter : public bit_io::BitWriter {
 public:
  ~StringBasedBitWriter() override = default;

  bool WriteBit(bool enabled) override {
    const uint32_t byte_index = bit_index_ / CHAR_BIT;
    if (byte_index >= content_.size()) {
      content_.push_back('\0');
    }
    content_[byte_index] = static_cast<char>(bits_manipulation::SetBitInByte(
        static_cast<std::byte>(content_[byte_index]), bit_index_ % CHAR_BIT,
        enabled));
    ++bit_index_;

    return true;
  }

  bool WriteFooter() override { return true; }

  const std::string& getContent() const { return content_; }

 private:
  uint32_t bit_index_{0};
  std::string content_;
};

}  // namespace test

#endif  // TEST_STRING_BIT_IO_H_
