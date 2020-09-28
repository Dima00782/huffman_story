#include "encryption/char_streams_adapters/char_ostream_adapter.h"
#include "gtest/gtest.h"

#include <memory>
#include <sstream>

TEST(CharOStreamAdapter, EmptyBits) {
  auto ostring_stream = std::make_shared<std::ostringstream>();
  { char_adapters::CharOStreamAdapter bit_writer{ostring_stream}; }

  EXPECT_TRUE(ostring_stream->str().empty());
}

TEST(CharOStreamAdapter, SevenUnusedBits) {
  auto ostring_stream = std::make_shared<std::ostringstream>();
  {
    char_adapters::CharOStreamAdapter bit_writer{ostring_stream};
    bit_writer.WriteBit(true);
  }
  EXPECT_EQ(ostring_stream->str(), "\x87");
}

TEST(CharOStreamAdapter, SixUnusedBits) {
  auto ostring_stream = std::make_shared<std::ostringstream>();
  {
    char_adapters::CharOStreamAdapter bit_writer{ostring_stream};
    bit_writer.WriteBit(false);
    bit_writer.WriteBit(true);
  }

  EXPECT_EQ(ostring_stream->str(), "\x46");
}

TEST(CharOStreamAdapter, FiveUnusedBits) {
  auto ostring_stream = std::make_shared<std::ostringstream>();
  {
    char_adapters::CharOStreamAdapter bit_writer{ostring_stream};
    bit_writer.WriteBit(true);
    bit_writer.WriteBit(true);
    bit_writer.WriteBit(false);
  }
  EXPECT_EQ(ostring_stream->str(), "\xc5");
}

TEST(CharOStreamAdapter, FourUnusedBits) {
  auto ostring_stream = std::make_shared<std::ostringstream>();
  {
    char_adapters::CharOStreamAdapter bit_writer{ostring_stream};
    bit_writer.WriteBit(true);
    bit_writer.WriteBit(false);
    bit_writer.WriteBit(true);
    bit_writer.WriteBit(true);
  }
  EXPECT_EQ(ostring_stream->str(), "\xb4");
}

TEST(CharOStreamAdapter, ThreeUnusedBits) {
  auto ostring_stream = std::make_shared<std::ostringstream>();
  {
    char_adapters::CharOStreamAdapter bit_writer{ostring_stream};
    bit_writer.WriteBit(true);
    bit_writer.WriteBit(false);
    bit_writer.WriteBit(true);
    bit_writer.WriteBit(true);
    bit_writer.WriteBit(false);
  }
  EXPECT_EQ(ostring_stream->str(), "\xb3");
}

TEST(CharOStreamAdapter, TwoUnusedBits) {
  auto ostring_stream = std::make_shared<std::ostringstream>();
  {
    char_adapters::CharOStreamAdapter bit_writer{ostring_stream};
    bit_writer.WriteBit(true);
    bit_writer.WriteBit(false);
    bit_writer.WriteBit(true);
    bit_writer.WriteBit(true);
    bit_writer.WriteBit(false);
    bit_writer.WriteBit(false);
  }
  EXPECT_EQ(ostring_stream->str(),
            "\xb0"
            "\x02");
}

TEST(CharOStreamAdapter, OneUnusedBit) {
  auto ostring_stream = std::make_shared<std::ostringstream>();
  {
    char_adapters::CharOStreamAdapter bit_writer{ostring_stream};
    bit_writer.WriteBit(true);
    bit_writer.WriteBit(false);
    bit_writer.WriteBit(true);
    bit_writer.WriteBit(true);
    bit_writer.WriteBit(false);
    bit_writer.WriteBit(false);
    bit_writer.WriteBit(true);
  }
  EXPECT_EQ(ostring_stream->str(),
            "\xb2"
            "\x01");
}
