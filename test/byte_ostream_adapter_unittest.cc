#include "encoding/byte_streams_adapters/byte_aligned_bit_writer.h"
#include "gtest/gtest.h"

#include <memory>
#include <sstream>

namespace {

TEST(ByteAlignedBitWriter, EmptyBits) {
  auto ostring_stream = std::make_shared<std::ostringstream>();
  {
    byte_adapters::ByteAlignedBitWriter bit_writer{ostring_stream};
    ASSERT_TRUE(bit_writer.WriteFooter());
  }

  EXPECT_TRUE(ostring_stream->str().empty());
}

TEST(ByteAlignedBitWriter, SevenUnusedBits) {
  auto ostring_stream = std::make_shared<std::ostringstream>();
  {
    byte_adapters::ByteAlignedBitWriter bit_writer{ostring_stream};
    ASSERT_TRUE(bit_writer.WriteBit(true));
    ASSERT_TRUE(bit_writer.WriteFooter());
  }
  EXPECT_EQ(ostring_stream->str(), "\x87");
}

TEST(ByteAlignedBitWriter, SixUnusedBits) {
  auto ostring_stream = std::make_shared<std::ostringstream>();
  {
    byte_adapters::ByteAlignedBitWriter bit_writer{ostring_stream};
    ASSERT_TRUE(bit_writer.WriteBit(false));
    ASSERT_TRUE(bit_writer.WriteBit(true));
    ASSERT_TRUE(bit_writer.WriteFooter());
  }

  EXPECT_EQ(ostring_stream->str(), "\x46");
}

TEST(ByteAlignedBitWriter, FiveUnusedBits) {
  auto ostring_stream = std::make_shared<std::ostringstream>();
  {
    byte_adapters::ByteAlignedBitWriter bit_writer{ostring_stream};
    ASSERT_TRUE(bit_writer.WriteBit(true));
    ASSERT_TRUE(bit_writer.WriteBit(true));
    ASSERT_TRUE(bit_writer.WriteBit(false));
    ASSERT_TRUE(bit_writer.WriteFooter());
  }
  EXPECT_EQ(ostring_stream->str(), "\xc5");
}

TEST(ByteAlignedBitWriter, FourUnusedBits) {
  auto ostring_stream = std::make_shared<std::ostringstream>();
  {
    byte_adapters::ByteAlignedBitWriter bit_writer{ostring_stream};
    ASSERT_TRUE(bit_writer.WriteBit(true));
    ASSERT_TRUE(bit_writer.WriteBit(false));
    ASSERT_TRUE(bit_writer.WriteBit(true));
    ASSERT_TRUE(bit_writer.WriteBit(true));
    ASSERT_TRUE(bit_writer.WriteFooter());
  }
  EXPECT_EQ(ostring_stream->str(), "\xb4");
}

TEST(ByteAlignedBitWriter, ThreeUnusedBits) {
  auto ostring_stream = std::make_shared<std::ostringstream>();
  {
    byte_adapters::ByteAlignedBitWriter bit_writer{ostring_stream};
    ASSERT_TRUE(bit_writer.WriteBit(true));
    ASSERT_TRUE(bit_writer.WriteBit(false));
    ASSERT_TRUE(bit_writer.WriteBit(true));
    ASSERT_TRUE(bit_writer.WriteBit(true));
    ASSERT_TRUE(bit_writer.WriteBit(false));
    ASSERT_TRUE(bit_writer.WriteFooter());
  }
  EXPECT_EQ(ostring_stream->str(), "\xb3");
}

TEST(ByteAlignedBitWriter, TwoUnusedBits) {
  auto ostring_stream = std::make_shared<std::ostringstream>();
  {
    byte_adapters::ByteAlignedBitWriter bit_writer{ostring_stream};
    ASSERT_TRUE(bit_writer.WriteBit(true));
    ASSERT_TRUE(bit_writer.WriteBit(false));
    ASSERT_TRUE(bit_writer.WriteBit(true));
    ASSERT_TRUE(bit_writer.WriteBit(true));
    ASSERT_TRUE(bit_writer.WriteBit(false));
    ASSERT_TRUE(bit_writer.WriteBit(false));
    ASSERT_TRUE(bit_writer.WriteFooter());
  }
  EXPECT_EQ(ostring_stream->str(),
            "\xb0"
            "\x02");
}

TEST(ByteAlignedBitWriter, OneUnusedBit) {
  auto ostring_stream = std::make_shared<std::ostringstream>();
  {
    byte_adapters::ByteAlignedBitWriter bit_writer{ostring_stream};
    ASSERT_TRUE(bit_writer.WriteBit(true));
    ASSERT_TRUE(bit_writer.WriteBit(false));
    ASSERT_TRUE(bit_writer.WriteBit(true));
    ASSERT_TRUE(bit_writer.WriteBit(true));
    ASSERT_TRUE(bit_writer.WriteBit(false));
    ASSERT_TRUE(bit_writer.WriteBit(false));
    ASSERT_TRUE(bit_writer.WriteBit(true));
    ASSERT_TRUE(bit_writer.WriteFooter());
  }
  EXPECT_EQ(ostring_stream->str(),
            "\xb2"
            "\x01");
}

}  // namespace