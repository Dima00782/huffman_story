#include "encryption/char_streams_adapters/char_aligned_bit_writer.h"
#include "gtest/gtest.h"

#include <memory>
#include <sstream>

TEST(CharAlignedBitWriter, EmptyBits) {
  auto ostring_stream = std::make_shared<std::ostringstream>();
  { char_adapters::CharAlignedBitWriter bit_writer{ostring_stream}; }

  EXPECT_TRUE(ostring_stream->str().empty());
}

TEST(CharAlignedBitWriter, SevenUnusedBits) {
  auto ostring_stream = std::make_shared<std::ostringstream>();
  {
    char_adapters::CharAlignedBitWriter bit_writer{ostring_stream};
    ASSERT_TRUE(bit_writer.WriteBit(true));
  }
  EXPECT_EQ(ostring_stream->str(), "\x87");
}

TEST(CharAlignedBitWriter, SixUnusedBits) {
  auto ostring_stream = std::make_shared<std::ostringstream>();
  {
    char_adapters::CharAlignedBitWriter bit_writer{ostring_stream};
    ASSERT_TRUE(bit_writer.WriteBit(false));
    ASSERT_TRUE(bit_writer.WriteBit(true));
  }

  EXPECT_EQ(ostring_stream->str(), "\x46");
}

TEST(CharAlignedBitWriter, FiveUnusedBits) {
  auto ostring_stream = std::make_shared<std::ostringstream>();
  {
    char_adapters::CharAlignedBitWriter bit_writer{ostring_stream};
    ASSERT_TRUE(bit_writer.WriteBit(true));
    ASSERT_TRUE(bit_writer.WriteBit(true));
    ASSERT_TRUE(bit_writer.WriteBit(false));
  }
  EXPECT_EQ(ostring_stream->str(), "\xc5");
}

TEST(CharAlignedBitWriter, FourUnusedBits) {
  auto ostring_stream = std::make_shared<std::ostringstream>();
  {
    char_adapters::CharAlignedBitWriter bit_writer{ostring_stream};
    ASSERT_TRUE(bit_writer.WriteBit(true));
    ASSERT_TRUE(bit_writer.WriteBit(false));
    ASSERT_TRUE(bit_writer.WriteBit(true));
    ASSERT_TRUE(bit_writer.WriteBit(true));
  }
  EXPECT_EQ(ostring_stream->str(), "\xb4");
}

TEST(CharAlignedBitWriter, ThreeUnusedBits) {
  auto ostring_stream = std::make_shared<std::ostringstream>();
  {
    char_adapters::CharAlignedBitWriter bit_writer{ostring_stream};
    ASSERT_TRUE(bit_writer.WriteBit(true));
    ASSERT_TRUE(bit_writer.WriteBit(false));
    ASSERT_TRUE(bit_writer.WriteBit(true));
    ASSERT_TRUE(bit_writer.WriteBit(true));
    ASSERT_TRUE(bit_writer.WriteBit(false));
  }
  EXPECT_EQ(ostring_stream->str(), "\xb3");
}

TEST(CharAlignedBitWriter, TwoUnusedBits) {
  auto ostring_stream = std::make_shared<std::ostringstream>();
  {
    char_adapters::CharAlignedBitWriter bit_writer{ostring_stream};
    ASSERT_TRUE(bit_writer.WriteBit(true));
    ASSERT_TRUE(bit_writer.WriteBit(false));
    ASSERT_TRUE(bit_writer.WriteBit(true));
    ASSERT_TRUE(bit_writer.WriteBit(true));
    ASSERT_TRUE(bit_writer.WriteBit(false));
    ASSERT_TRUE(bit_writer.WriteBit(false));
  }
  EXPECT_EQ(ostring_stream->str(),
            "\xb0"
            "\x02");
}

TEST(CharAlignedBitWriter, OneUnusedBit) {
  auto ostring_stream = std::make_shared<std::ostringstream>();
  {
    char_adapters::CharAlignedBitWriter bit_writer{ostring_stream};
    ASSERT_TRUE(bit_writer.WriteBit(true));
    ASSERT_TRUE(bit_writer.WriteBit(false));
    ASSERT_TRUE(bit_writer.WriteBit(true));
    ASSERT_TRUE(bit_writer.WriteBit(true));
    ASSERT_TRUE(bit_writer.WriteBit(false));
    ASSERT_TRUE(bit_writer.WriteBit(false));
    ASSERT_TRUE(bit_writer.WriteBit(true));
  }
  EXPECT_EQ(ostring_stream->str(),
            "\xb2"
            "\x01");
}
