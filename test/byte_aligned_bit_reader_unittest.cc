#include "encryption/byte_aligned_bit_reader.h"
#include "gtest/gtest.h"

#include <cassert>
#include <memory>
#include <string>
#include <vector>

namespace {
class StringBitReader : public encryption::BitReader {
 public:
  explicit StringBitReader(const std::string& string_of_bits)
      : string_of_bits_{string_of_bits}, pos_{string_of_bits_.cbegin()} {}

  std::optional<bool> ReadBit() override {
    if (pos_ == string_of_bits_.cend()) {
      return std::nullopt;
    }
    assert(*pos_ == '1' || *pos_ == '0');
    const bool result = *pos_ == '1';
    ++pos_;
    return result;
  }

  ~StringBitReader() override = default;

 private:
  const std::string string_of_bits_;
  std::string::const_iterator pos_;
};

void CheckBitReaderForValues(const std::vector<bool>& values,
                             encryption::ByteAlignedBitReader* bit_reader) {
  for (const auto val : values) {
    auto bit = bit_reader->ReadBit();
    EXPECT_TRUE(bit && *bit == val);
  }
}
}  // namespace

TEST(ByteAlignedBitReader, EmptyBits) {
  encryption::ByteAlignedBitReader bit_reader{
      std::make_unique<StringBitReader>("")};
  EXPECT_FALSE(bit_reader.ReadBit());
}

TEST(ByteAlignedBitReader, OneUnsedBit) {
  encryption::ByteAlignedBitReader bit_reader{
      std::make_unique<StringBitReader>("01101010"
                                        "00000001")};

  CheckBitReaderForValues({false, true, true, false, true, false, true},
                          &bit_reader);
  EXPECT_FALSE(bit_reader.ReadBit());
}

TEST(ByteAlignedBitReader, TwoUnsedBits) {
  encryption::ByteAlignedBitReader bit_reader{
      std::make_unique<StringBitReader>("00010100"
                                        "00000010")};

  CheckBitReaderForValues({false, false, false, true, false, true},
                          &bit_reader);
  EXPECT_FALSE(bit_reader.ReadBit());
}

TEST(ByteAlignedBitReader, ThreeUnsedBits) {
  encryption::ByteAlignedBitReader bit_reader{
      std::make_unique<StringBitReader>("11101011")};

  CheckBitReaderForValues({true, true, true, false, true}, &bit_reader);
  EXPECT_FALSE(bit_reader.ReadBit());
}

TEST(ByteAlignedBitReader, FourUnsedBits) {
  encryption::ByteAlignedBitReader bit_reader{
      std::make_unique<StringBitReader>("01010100")};

  CheckBitReaderForValues({false, true, false, true}, &bit_reader);
  EXPECT_FALSE(bit_reader.ReadBit());
}

TEST(ByteAlignedBitReader, FiveUnsedBits) {
  encryption::ByteAlignedBitReader bit_reader{
      std::make_unique<StringBitReader>("00000101")};

  CheckBitReaderForValues({false, false, false}, &bit_reader);
  EXPECT_FALSE(bit_reader.ReadBit());
}

TEST(ByteAlignedBitReader, SixUnsedBits) {
  encryption::ByteAlignedBitReader bit_reader{
      std::make_unique<StringBitReader>("10000110")};

  CheckBitReaderForValues({true, false}, &bit_reader);
  EXPECT_FALSE(bit_reader.ReadBit());
}

TEST(ByteAlignedBitReader, SevenUnsedBits) {
  encryption::ByteAlignedBitReader bit_reader{
      std::make_unique<StringBitReader>("10000111")};

  CheckBitReaderForValues({true}, &bit_reader);
  EXPECT_FALSE(bit_reader.ReadBit());
}

TEST(ByteAlignedBitReader, TwoFullBytesOfData) {
  encryption::ByteAlignedBitReader bit_reader{
      std::make_unique<StringBitReader>("10011101"
                                        "00101011"
                                        "00000000")};

  CheckBitReaderForValues({true, false, false, true, true, true, false, true,
                           false, false, true, false, true, false, true, true},
                          &bit_reader);
  EXPECT_FALSE(bit_reader.ReadBit());
}
