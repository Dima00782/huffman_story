#include "letter/single_byte_letter/single_byte_letter.h"
#include "gtest/gtest.h"
#include "test/string_bit_io.h"

#include <climits>
#include <memory>
#include <sstream>
#include <utility>

namespace single_byte_letter {

TEST(SingleByteLetter, Parser) {
  using Parser = SingleByteLetterConfig::LetterParser;
  auto input = std::make_shared<std::istringstream>(" abc defg ");
  Parser parser(std::move(input));
  EXPECT_EQ(*parser.Parse(), std::byte(' '));
  EXPECT_EQ(*parser.Parse(), std::byte('a'));
  EXPECT_EQ(*parser.Parse(), std::byte('b'));
  EXPECT_EQ(*parser.Parse(), std::byte('c'));
  EXPECT_EQ(*parser.Parse(), std::byte(' '));
  EXPECT_EQ(*parser.Parse(), std::byte('d'));
  EXPECT_EQ(*parser.Parse(), std::byte('e'));
  EXPECT_EQ(*parser.Parse(), std::byte('f'));
  EXPECT_EQ(*parser.Parse(), std::byte('g'));
  EXPECT_EQ(*parser.Parse(), std::byte(' '));
  EXPECT_FALSE(parser.Parse());
}

TEST(SingleByteLetter, SerializerRead) {
  SingleByteLetterConfig config;
  test::StringBasedBitReader input("abc");
  EXPECT_EQ(*config.ReadSerialized(input), std::byte('a'));
  EXPECT_EQ(*config.ReadSerialized(input), std::byte('b'));
  EXPECT_EQ(*config.ReadSerialized(input), std::byte('c'));
  EXPECT_FALSE(config.ReadSerialized(input));
}

TEST(SingleByteLetter, SerializerWrite) {
  SingleByteLetterConfig config;
  test::StringBasedBitWriter output;
  ASSERT_TRUE(config.WriteSerialized(output, std::byte{'a'}));
  ASSERT_TRUE(config.WriteSerialized(output, std::byte{'b'}));
  ASSERT_TRUE(config.WriteSerialized(output, std::byte{'c'}));
  EXPECT_EQ(output.getContent(), "abc");
}

}  // namespace single_byte_letter