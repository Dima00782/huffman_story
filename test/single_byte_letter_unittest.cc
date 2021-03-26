#include "letter/single_byte_letter/single_byte_letter.h"
#include "gtest/gtest.h"
#include "test/string_bit_io.h"

#include <climits>
#include <memory>
#include <sstream>
#include <utility>

namespace single_byte_letter {

using Letter = SingleByteLetterConfig::LetterType;

TEST(SingleByteLetter, Parser) {
  SingleByteLetterConfig config;
  auto input = std::make_shared<std::istringstream>(" abc defg ");
  auto parser = config.CreateParser(std::move(input));
  EXPECT_EQ(*parser->Parse(), Letter(' '));
  EXPECT_EQ(*parser->Parse(), Letter('a'));
  EXPECT_EQ(*parser->Parse(), Letter('b'));
  EXPECT_EQ(*parser->Parse(), Letter('c'));
  EXPECT_EQ(*parser->Parse(), Letter(' '));
  EXPECT_EQ(*parser->Parse(), Letter('d'));
  EXPECT_EQ(*parser->Parse(), Letter('e'));
  EXPECT_EQ(*parser->Parse(), Letter('f'));
  EXPECT_EQ(*parser->Parse(), Letter('g'));
  EXPECT_EQ(*parser->Parse(), Letter(' '));
  EXPECT_FALSE(parser->Parse());
}

TEST(SingleByteLetter, SerializerRead) {
  SingleByteLetterConfig config;
  test::StringBasedBitReader input("abc");
  EXPECT_EQ(*config.ReadSerialized(input), Letter('a'));
  EXPECT_EQ(*config.ReadSerialized(input), Letter('b'));
  EXPECT_EQ(*config.ReadSerialized(input), Letter('c'));
  EXPECT_FALSE(config.ReadSerialized(input));
}

TEST(SingleByteLetter, SerializerWrite) {
  SingleByteLetterConfig config;
  test::StringBasedBitWriter output;
  ASSERT_TRUE(config.WriteSerialized(output, Letter{'a'}));
  ASSERT_TRUE(config.WriteSerialized(output, Letter{'b'}));
  ASSERT_TRUE(config.WriteSerialized(output, Letter{'c'}));
  EXPECT_EQ(output.getContent(), "abc");
}

// TODO: test Write method!

}  // namespace single_byte_letter