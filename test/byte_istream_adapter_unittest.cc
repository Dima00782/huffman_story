#include "encoding/byte_streams_adapters/byte_aligned_bit_reader.h"
#include "gtest/gtest.h"

#include <memory>
#include <sstream>

namespace {

TEST(ByteAlignedBitReader, SevenUnusedBits) {
  auto istring_stream = std::make_shared<std::istringstream>("\x87");
  byte_adapters::ByteAlignedBitReader bit_reader{std::move(istring_stream)};

  EXPECT_TRUE(*bit_reader.ReadBit());
  EXPECT_FALSE(bit_reader.ReadBit());
}

TEST(ByteAlignedBitReader, SixUnusedBits) {
  auto istring_stream = std::make_shared<std::istringstream>("\xc6");
  byte_adapters::ByteAlignedBitReader bit_reader{std::move(istring_stream)};

  EXPECT_TRUE(*bit_reader.ReadBit());
  EXPECT_TRUE(*bit_reader.ReadBit());
  EXPECT_FALSE(bit_reader.ReadBit());
}

TEST(ByteAlignedBitReader, FiveUnusedBits) {
  auto istring_stream = std::make_shared<std::istringstream>("\xa5");
  byte_adapters::ByteAlignedBitReader bit_reader{std::move(istring_stream)};

  EXPECT_TRUE(*bit_reader.ReadBit());
  EXPECT_FALSE(*bit_reader.ReadBit());
  EXPECT_TRUE(*bit_reader.ReadBit());

  EXPECT_FALSE(bit_reader.ReadBit());
}

TEST(ByteAlignedBitReader, FourUnusedBits) {
  auto istring_stream = std::make_shared<std::istringstream>("\xb4");
  byte_adapters::ByteAlignedBitReader bit_reader{std::move(istring_stream)};

  EXPECT_TRUE(*bit_reader.ReadBit());
  EXPECT_FALSE(*bit_reader.ReadBit());
  EXPECT_TRUE(*bit_reader.ReadBit());
  EXPECT_TRUE(*bit_reader.ReadBit());

  EXPECT_FALSE(bit_reader.ReadBit());
}

TEST(ByteAlignedBitReader, ThreeUnusedBits) {
  auto istring_stream = std::make_shared<std::istringstream>("\x5b");
  byte_adapters::ByteAlignedBitReader bit_reader{std::move(istring_stream)};

  EXPECT_FALSE(*bit_reader.ReadBit());
  EXPECT_TRUE(*bit_reader.ReadBit());
  EXPECT_FALSE(*bit_reader.ReadBit());
  EXPECT_TRUE(*bit_reader.ReadBit());
  EXPECT_TRUE(*bit_reader.ReadBit());

  EXPECT_FALSE(bit_reader.ReadBit());
}

TEST(ByteAlignedBitReader, TwoUnusedBits) {
  auto istring_stream = std::make_shared<std::istringstream>("\xa8\x02");
  byte_adapters::ByteAlignedBitReader bit_reader{std::move(istring_stream)};

  EXPECT_TRUE(*bit_reader.ReadBit());
  EXPECT_FALSE(*bit_reader.ReadBit());
  EXPECT_TRUE(*bit_reader.ReadBit());
  EXPECT_FALSE(*bit_reader.ReadBit());
  EXPECT_TRUE(*bit_reader.ReadBit());
  EXPECT_FALSE(*bit_reader.ReadBit());

  EXPECT_FALSE(bit_reader.ReadBit());
}

TEST(ByteAlignedBitReader, OneUnusedBits) {
  auto istring_stream = std::make_shared<std::istringstream>("\xaa\x01");
  byte_adapters::ByteAlignedBitReader bit_reader{std::move(istring_stream)};

  EXPECT_TRUE(*bit_reader.ReadBit());
  EXPECT_FALSE(*bit_reader.ReadBit());
  EXPECT_TRUE(*bit_reader.ReadBit());
  EXPECT_FALSE(*bit_reader.ReadBit());
  EXPECT_TRUE(*bit_reader.ReadBit());
  EXPECT_FALSE(*bit_reader.ReadBit());
  EXPECT_TRUE(*bit_reader.ReadBit());

  EXPECT_FALSE(bit_reader.ReadBit());
}

TEST(ByteAlignedBitReader, ZeroUnusedBits) {
  auto istring_stream =
      std::make_shared<std::istringstream>(std::string("\xaa\x00", 2));
  byte_adapters::ByteAlignedBitReader bit_reader{std::move(istring_stream)};

  EXPECT_TRUE(*bit_reader.ReadBit());
  EXPECT_FALSE(*bit_reader.ReadBit());
  EXPECT_TRUE(*bit_reader.ReadBit());
  EXPECT_FALSE(*bit_reader.ReadBit());
  EXPECT_TRUE(*bit_reader.ReadBit());
  EXPECT_FALSE(*bit_reader.ReadBit());
  EXPECT_TRUE(*bit_reader.ReadBit());
  EXPECT_FALSE(*bit_reader.ReadBit());

  EXPECT_FALSE(bit_reader.ReadBit());
}

}  // namespace