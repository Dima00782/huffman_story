#include "encryption/byte_aligned_bit_writer.h"
#include "gtest/gtest.h"

#include <memory>
#include <string>
#include <vector>

namespace {
class StringBitWriter : public encryption::BitWriter {
 public:
  explicit StringBitWriter(std::string* buffer) : buffer_{buffer} {}

  void WriteBit(bool enabled) override {
    const char symbol = enabled ? '1' : '0';
    buffer_->push_back(symbol);
  }

  ~StringBitWriter() override = default;

 private:
  std::string* buffer_;
};
}  // namespace

TEST(ByteAlignedBitWriter, NoBits) {
  std::string buffer;
  {
    encryption::ByteAlignedBitWriter bit_writer{
        std::make_unique<StringBitWriter>(&buffer)};
  }

  EXPECT_TRUE(buffer.empty());
}

TEST(ByteAlignedBitWriter, OneBit) {
  std::string buffer;
  {
    encryption::ByteAlignedBitWriter bit_writer{
        std::make_unique<StringBitWriter>(&buffer)};
    bit_writer.WriteBit(true);
  }

  EXPECT_EQ("10000111", buffer);
}

TEST(ByteAlignedBitWriter, TwoBits) {
  std::string buffer;
  {
    encryption::ByteAlignedBitWriter bit_writer{
        std::make_unique<StringBitWriter>(&buffer)};
    bit_writer.WriteBit(false);
    bit_writer.WriteBit(true);
  }

  EXPECT_EQ("01000110", buffer);
}

TEST(ByteAlignedBitWriter, ThreeBits) {
  std::string buffer;
  {
    encryption::ByteAlignedBitWriter bit_writer{
        std::make_unique<StringBitWriter>(&buffer)};
    bit_writer.WriteBit(true);
    bit_writer.WriteBit(false);
    bit_writer.WriteBit(true);
  }

  EXPECT_EQ("10100101", buffer);
}

TEST(ByteAlignedBitWriter, FourBits) {
  std::string buffer;
  {
    encryption::ByteAlignedBitWriter bit_writer{
        std::make_unique<StringBitWriter>(&buffer)};
    bit_writer.WriteBit(true);
    bit_writer.WriteBit(false);
    bit_writer.WriteBit(true);
    bit_writer.WriteBit(true);
  }

  EXPECT_EQ("10110100", buffer);
}

TEST(ByteAlignedBitWriter, FiveBits) {
  std::string buffer;
  {
    encryption::ByteAlignedBitWriter bit_writer{
        std::make_unique<StringBitWriter>(&buffer)};
    bit_writer.WriteBit(true);
    bit_writer.WriteBit(false);
    bit_writer.WriteBit(true);
    bit_writer.WriteBit(false);
    bit_writer.WriteBit(true);
  }

  EXPECT_EQ("10101011", buffer);
}

TEST(ByteAlignedBitWriter, SixBits) {
  std::string buffer;
  {
    encryption::ByteAlignedBitWriter bit_writer{
        std::make_unique<StringBitWriter>(&buffer)};
    bit_writer.WriteBit(true);
    bit_writer.WriteBit(false);
    bit_writer.WriteBit(true);
    bit_writer.WriteBit(false);
    bit_writer.WriteBit(true);
    bit_writer.WriteBit(true);
  }

  EXPECT_EQ(
      "10101100"
      "00000010",
      buffer);
}

TEST(ByteAlignedBitWriter, SevenBits) {
  std::string buffer;
  {
    encryption::ByteAlignedBitWriter bit_writer{
        std::make_unique<StringBitWriter>(&buffer)};
    bit_writer.WriteBit(true);
    bit_writer.WriteBit(false);
    bit_writer.WriteBit(true);
    bit_writer.WriteBit(false);
    bit_writer.WriteBit(true);
    bit_writer.WriteBit(false);
    bit_writer.WriteBit(true);
  }

  EXPECT_EQ(
      "10101010"
      "00000001",
      buffer);
}

TEST(ByteAlignedBitWriter, EightBits) {
  std::string buffer;
  {
    encryption::ByteAlignedBitWriter bit_writer{
        std::make_unique<StringBitWriter>(&buffer)};
    bit_writer.WriteBit(true);
    bit_writer.WriteBit(false);
    bit_writer.WriteBit(true);
    bit_writer.WriteBit(false);
    bit_writer.WriteBit(true);
    bit_writer.WriteBit(false);
    bit_writer.WriteBit(true);
    bit_writer.WriteBit(true);
  }

  EXPECT_EQ(
      "10101011"
      "00000000",
      buffer);
}
