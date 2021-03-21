#include "letter/fixed_alphabet_letter/fixed_alphabet_letter.h"
#include "gtest/gtest.h"

#include <climits>
#include <memory>
#include <sstream>
#include <unordered_map>
#include <utility>

namespace letter {

TEST(FixedAlphabetLetter, Parser) {
  const std::set<std::string> alphabet = {"ab", "def"};
  auto input = std::make_shared<std::istringstream>(" abc def abcdefggg");
  FixedAlphabetLetterParser parser(std::move(alphabet), std::move(input));
}

}  // namespace letter