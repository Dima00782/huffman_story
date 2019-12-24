#include "encryption/byte_aligned_bit_writer.h"
#include "gtest/gtest.h"

namespace {
class StringBitWriter : public encryption::BitWriter {
 public:
  explicit StringBitWriter(std::string* buffer) : buffer_{buffer} {}

  ~StringBitWriter() override = default;

  void WriteBit(bool enabled) override {
    buffer_->push_back(enabled ? '1' : '0');
  }

 private:
  std::string* buffer_;
};
}  // namespace

TEST(ByteAlignedBitWriter, EmptyBits) {
  std::string buffer;
  {
    encryption::ByteAlignedBitWriter bit_writer{
        std::make_unique<StringBitWriter>(&buffer)};
  }
  EXPECT_TRUE(buffer.empty());
}

TEST(ByteAlignedBitWriter, SevenUnsedBits) {
  std::string buffer;
  {
    encryption::ByteAlignedBitWriter bit_writer{
        std::make_unique<StringBitWriter>(&buffer)};
    bit_writer.WriteBit(true);
  }
  EXPECT_EQ(buffer, "10000111");
}

TEST(ByteAlignedBitWriter, SixUnsedBits) {
  std::string buffer;
  {
    encryption::ByteAlignedBitWriter bit_writer{
        std::make_unique<StringBitWriter>(&buffer)};
    bit_writer.WriteBit(false);
    bit_writer.WriteBit(true);
  }
  EXPECT_EQ(buffer, "01000110");
}

TEST(ByteAlignedBitWriter, FiveUnsedBits) {
  std::string buffer;
  {
    encryption::ByteAlignedBitWriter bit_writer{
        std::make_unique<StringBitWriter>(&buffer)};
    bit_writer.WriteBit(true);
    bit_writer.WriteBit(true);
    bit_writer.WriteBit(false);
  }
  EXPECT_EQ(buffer, "11000101");
}

TEST(ByteAlignedBitWriter, FourUnsedBits) {
  std::string buffer;
  {
    encryption::ByteAlignedBitWriter bit_writer{
        std::make_unique<StringBitWriter>(&buffer)};
    bit_writer.WriteBit(true);
    bit_writer.WriteBit(false);
    bit_writer.WriteBit(true);
    bit_writer.WriteBit(true);
  }
  EXPECT_EQ(buffer, "10110100");
}

TEST(ByteAlignedBitWriter, ThreeUnsedBits) {
  std::string buffer;
  {
    encryption::ByteAlignedBitWriter bit_writer{
        std::make_unique<StringBitWriter>(&buffer)};
    bit_writer.WriteBit(true);
    bit_writer.WriteBit(false);
    bit_writer.WriteBit(true);
    bit_writer.WriteBit(true);
    bit_writer.WriteBit(false);
  }
  EXPECT_EQ(buffer, "10110011");
}

TEST(ByteAlignedBitWriter, TwoUnsedBits) {
  std::string buffer;
  {
    encryption::ByteAlignedBitWriter bit_writer{
        std::make_unique<StringBitWriter>(&buffer)};
    bit_writer.WriteBit(true);
    bit_writer.WriteBit(false);
    bit_writer.WriteBit(true);
    bit_writer.WriteBit(true);
    bit_writer.WriteBit(false);
    bit_writer.WriteBit(false);
  }
  EXPECT_EQ(buffer,
            "10110000"
            "00000010");
}

TEST(ByteAlignedBitWriter, OneUnsedBit) {
  std::string buffer;
  {
    encryption::ByteAlignedBitWriter bit_writer{
        std::make_unique<StringBitWriter>(&buffer)};
    bit_writer.WriteBit(true);
    bit_writer.WriteBit(false);
    bit_writer.WriteBit(true);
    bit_writer.WriteBit(true);
    bit_writer.WriteBit(false);
    bit_writer.WriteBit(false);
    bit_writer.WriteBit(true);
  }
  EXPECT_EQ(buffer,
            "10110010"
            "00000001");
}
