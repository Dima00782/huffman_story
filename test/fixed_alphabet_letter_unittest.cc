#include "letter/fixed_alphabet_letter/fixed_alphabet_letter.h"
#include "gtest/gtest.h"
#include "test/string_bit_io.h"

#include <memory>
#include <set>
#include <sstream>
#include <string>

using namespace std::string_literals;

namespace fixed_alpha_letter {
namespace {

using Letter = FixedAlphabetLetterConfig::LetterType;

TEST(FixedAlphabetLetter, Parser) {
  FixedAlphabetLetterConfig config({"ab", "def"});
  auto input = std::make_shared<std::istringstream>(" abc def abcdefgggz");
  auto parser = config.CreateParser(std::move(input));

  EXPECT_EQ(*parser->Parse(), Letter{" "});
  EXPECT_EQ(*parser->Parse(), Letter{"ab"});
  EXPECT_EQ(*parser->Parse(), Letter{"c"});
  EXPECT_EQ(*parser->Parse(), Letter{" "});
  EXPECT_EQ(*parser->Parse(), Letter{"def"});
  EXPECT_EQ(*parser->Parse(), Letter{" "});
  EXPECT_EQ(*parser->Parse(), Letter{"ab"});
  EXPECT_EQ(*parser->Parse(), Letter{"c"});
  EXPECT_EQ(*parser->Parse(), Letter{"def"});
  EXPECT_EQ(*parser->Parse(), Letter{"g"});
  EXPECT_EQ(*parser->Parse(), Letter{"g"});
  EXPECT_EQ(*parser->Parse(), Letter{"g"});
  EXPECT_EQ(*parser->Parse(), Letter{"z"});
  EXPECT_FALSE(parser->Parse());
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
  EXPECT_EQ(*config.ReadSerialized(input), Letter{" "});
  EXPECT_EQ(*config.ReadSerialized(input), Letter{"ab"});
  EXPECT_EQ(*config.ReadSerialized(input), Letter{" "});
  EXPECT_EQ(*config.ReadSerialized(input), Letter{"def"});
  EXPECT_EQ(*config.ReadSerialized(input), Letter{" "});
  EXPECT_EQ(*config.ReadSerialized(input), Letter{"ab"});
  EXPECT_EQ(*config.ReadSerialized(input), Letter{"def"});
  EXPECT_EQ(*config.ReadSerialized(input), Letter{"gg"});
  EXPECT_EQ(*config.ReadSerialized(input), Letter{"g"});
  EXPECT_FALSE(config.ReadSerialized(input));
}

TEST(FixedAlphabetLetter, SerializerWrite) {
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

TEST(FixedAlphabetLetter, Write) {
  FixedAlphabetLetterConfig config({""});
  std::ostringstream output;

  ASSERT_TRUE(config.Write(output, Letter{"abc"}));
  ASSERT_TRUE(config.Write(output, Letter{" def"}));
  ASSERT_TRUE(config.Write(output, Letter{"ggg"}));
  ASSERT_TRUE(config.Write(output, Letter{" "}));
  EXPECT_EQ(output.str(), "abc defggg ");
}

}  // namespace
}  // namespace fixed_alpha_letter