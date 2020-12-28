#include "gtest/gtest.h"
#include "letter/one_byte_letter.h"

#include <memory>
#include <unordered_map>
#include <utility>

namespace letter {

TEST(OneByteLetter, Trivial) {
  auto letter = std::make_unique<OneByteLetter>(std::byte{'a'});
}

TEST(OneByteLetter, Hashable) {
  std::unordered_map<std::unique_ptr<Letter>, int> map;
  map.insert(std::pair(std::make_unique<OneByteLetter>(std::byte{'a'}), 10));
}

}  // namespace letter