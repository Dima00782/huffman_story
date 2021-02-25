#include "encoding/char_streams_adapters/char_aligned_bit_reader.h"
#include "gtest/gtest.h"

#include <memory>
#include <sstream>

TEST(CharAlignedBitReader, SevenUnusedBits) {
  auto istring_stream = std::make_shared<std::istringstream>("\x87");
  char_adapters::CharAlignedBitReader bit_reader{std::move(istring_stream)};

  EXPECT_TRUE(*bit_reader.ReadBit());
  EXPECT_FALSE(bit_reader.ReadBit());
}

TEST(CharAlignedBitReader, SixUnusedBits) {
  auto istring_stream = std::make_shared<std::istringstream>("\xc6");
  char_adapters::CharAlignedBitReader bit_reader{std::move(istring_stream)};

  EXPECT_TRUE(*bit_reader.ReadBit());
  EXPECT_TRUE(*bit_reader.ReadBit());
  EXPECT_FALSE(bit_reader.ReadBit());
}

TEST(CharAlignedBitReader, FiveUnusedBits) {
  auto istring_stream = std::make_shared<std::istringstream>("\xa5");
  char_adapters::CharAlignedBitReader bit_reader{std::move(istring_stream)};

  EXPECT_TRUE(*bit_reader.ReadBit());
  EXPECT_FALSE(*bit_reader.ReadBit());
  EXPECT_TRUE(*bit_reader.ReadBit());

  EXPECT_FALSE(bit_reader.ReadBit());
}

TEST(CharAlignedBitReader, FourUnusedBits) {
  auto istring_stream = std::make_shared<std::istringstream>("\xb4");
  char_adapters::CharAlignedBitReader bit_reader{std::move(istring_stream)};

  EXPECT_TRUE(*bit_reader.ReadBit());
  EXPECT_FALSE(*bit_reader.ReadBit());
  EXPECT_TRUE(*bit_reader.ReadBit());
  EXPECT_TRUE(*bit_reader.ReadBit());

  EXPECT_FALSE(bit_reader.ReadBit());
}

TEST(CharAlignedBitReader, ThreeUnusedBits) {
  auto istring_stream = std::make_shared<std::istringstream>("\x5b");
  char_adapters::CharAlignedBitReader bit_reader{std::move(istring_stream)};

  EXPECT_FALSE(*bit_reader.ReadBit());
  EXPECT_TRUE(*bit_reader.ReadBit());
  EXPECT_FALSE(*bit_reader.ReadBit());
  EXPECT_TRUE(*bit_reader.ReadBit());
  EXPECT_TRUE(*bit_reader.ReadBit());

  EXPECT_FALSE(bit_reader.ReadBit());
}

TEST(CharAlignedBitReader, TwoUnusedBits) {
  auto istring_stream = std::make_shared<std::istringstream>("\xa8\x02");
  char_adapters::CharAlignedBitReader bit_reader{std::move(istring_stream)};

  EXPECT_TRUE(*bit_reader.ReadBit());
  EXPECT_FALSE(*bit_reader.ReadBit());
  EXPECT_TRUE(*bit_reader.ReadBit());
  EXPECT_FALSE(*bit_reader.ReadBit());
  EXPECT_TRUE(*bit_reader.ReadBit());
  EXPECT_FALSE(*bit_reader.ReadBit());

  EXPECT_FALSE(bit_reader.ReadBit());
}

TEST(CharAlignedBitReader, OneUnusedBits) {
  auto istring_stream = std::make_shared<std::istringstream>("\xaa\x01");
  char_adapters::CharAlignedBitReader bit_reader{std::move(istring_stream)};

  EXPECT_TRUE(*bit_reader.ReadBit());
  EXPECT_FALSE(*bit_reader.ReadBit());
  EXPECT_TRUE(*bit_reader.ReadBit());
  EXPECT_FALSE(*bit_reader.ReadBit());
  EXPECT_TRUE(*bit_reader.ReadBit());
  EXPECT_FALSE(*bit_reader.ReadBit());
  EXPECT_TRUE(*bit_reader.ReadBit());

  EXPECT_FALSE(bit_reader.ReadBit());
}

TEST(CharAlignedBitReader, ZeroUnusedBits) {
  auto istring_stream =
      std::make_shared<std::istringstream>(std::string("\xaa\x00", 2));
  char_adapters::CharAlignedBitReader bit_reader{std::move(istring_stream)};

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
