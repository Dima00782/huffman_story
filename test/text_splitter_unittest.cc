#include "encryption/text_splitter/text_splitter.h"
#include "gtest/gtest.h"

namespace text_splitter {

TEST(TextSplitter, OneLetterAlphabet) {
  TextSplitter splitter({"a"});
  auto result = splitter.Split("aaa");
  EXPECT_EQ(result.size(), 3u);

  EXPECT_EQ(result[0], "a");
  EXPECT_EQ(result[1], "a");
  EXPECT_EQ(result[2], "a");
}

TEST(TextSplitter, ChooseLetterWithMaxLength) {
  {
    TextSplitter splitter({"a", "b", "aab"});
    auto result = splitter.Split("aabaab");
    EXPECT_EQ(result.size(), 2u);

    EXPECT_EQ(result[0], "aab");
    EXPECT_EQ(result[1], "aab");
  }

  {
    TextSplitter splitter({"a", "b", "aab"});
    auto result = splitter.Split("aababa");
    EXPECT_EQ(result.size(), 4u);

    EXPECT_EQ(result[0], "aab");
    EXPECT_EQ(result[1], "a");
    EXPECT_EQ(result[2], "b");
    EXPECT_EQ(result[3], "a");
  }
}

}  // namespace text_splitter
