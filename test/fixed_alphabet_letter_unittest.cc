#include "letter/fixed_alphabet_letter/fixed_alphabet_letter.h"
#include "gtest/gtest.h"

#include <memory>
#include <set>
#include <sstream>
#include <string>

using namespace std::string_literals;

namespace fixed_alpha_letter {

TEST(FixedAlphabetLetter, Parser) {
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

}  // namespace fixed_alpha_letter