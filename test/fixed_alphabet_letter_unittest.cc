#include "letter/fixed_alphabet_letter/fixed_alphabet_letter.h"
#include "gtest/gtest.h"
#include "test/string_bit_io.h"

#include <memory>
#include <set>
#include <sstream>
#include <string>

using namespace std::string_literals;

namespace fixed_alpha_letter {

TEST(FixedAlphabetLetter, Parser) {
  // TODO: rewrite via config.
  using Parser = FixedAlphabetLetterConfig::LetterParser;

  const std::set<std::string> alphabet = {"ab", "def"};
  auto input = std::make_shared<std::istringstream>(" abc def abcdefggg");
  Parser parser(std::move(alphabet), std::move(input));

  EXPECT_EQ(*parser.Parse(), " "s);
  EXPECT_EQ(*parser.Parse(), "ab"s);
  EXPECT_EQ(*parser.Parse(), "c"s);
  EXPECT_EQ(*parser.Parse(), " "s);
  EXPECT_EQ(*parser.Parse(), "def"s);
  EXPECT_EQ(*parser.Parse(), " "s);
  EXPECT_EQ(*parser.Parse(), "ab"s);
  EXPECT_EQ(*parser.Parse(), "c"s);
  EXPECT_EQ(*parser.Parse(), "def"s);
  EXPECT_EQ(*parser.Parse(), "g"s);
  EXPECT_EQ(*parser.Parse(), "g"s);
  EXPECT_EQ(*parser.Parse(), "g"s);
  EXPECT_FALSE(parser.Parse());
}

TEST(FixedAlphabetLetter, SerializerRead) {
  FixedAlphabetLetterConfig config({"ab", "def", "gg"});
  test::StringBasedBitReader input(
      std::string("\x01"
                  " "
                  "\x02"
                  "ab"
                  "\x01"
                  " "
                  "\x03"
                  "def"
                  "\x01"
                  " "
                  "\x02"
                  "ab"
                  "\x03"
                  "def"
                  "\x02"
                  "gg"
                  "\x01"
                  "g",
                  25));
  EXPECT_EQ(*config.ReadSerialized(input), " "s);
  EXPECT_EQ(*config.ReadSerialized(input), "ab"s);
  EXPECT_EQ(*config.ReadSerialized(input), " "s);
  EXPECT_EQ(*config.ReadSerialized(input), "def"s);
  EXPECT_EQ(*config.ReadSerialized(input), " "s);
  EXPECT_EQ(*config.ReadSerialized(input), "ab"s);
  EXPECT_EQ(*config.ReadSerialized(input), "def"s);
  EXPECT_EQ(*config.ReadSerialized(input), "gg"s);
  EXPECT_EQ(*config.ReadSerialized(input), "g"s);
  EXPECT_FALSE(config.ReadSerialized(input));
}

TEST(FixedAlphabetLetter, SerializerWrite) {
  using Letter = FixedAlphabetLetterConfig::LetterType;
  FixedAlphabetLetterConfig config({""});
  test::StringBasedBitWriter output;

  ASSERT_TRUE(config.WriteSerialized(output, Letter{"abc"}));
  ASSERT_TRUE(config.WriteSerialized(output, Letter{" def"}));
  ASSERT_TRUE(config.WriteSerialized(output, Letter{"ggg"}));
  ASSERT_TRUE(config.WriteSerialized(output, Letter{" "}));
  EXPECT_EQ(output.getContent(),
            "\x03"
            "abc"
            "\x04"
            " def"
            "\x3"
            "ggg"
            "\x1"
            " ");
}

}  // namespace fixed_alpha_letter