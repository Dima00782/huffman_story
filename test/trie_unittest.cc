#include "encryption/text_splitter/trie.h"
#include "gtest/gtest.h"

namespace text_splitter {

TEST(Trie, AddTrivial) {
  Trie trie;
  constexpr char kTestWord[] = "aaa";
  trie.Add(kTestWord);

  EXPECT_EQ(trie.GetLongestMatchingWord(kTestWord), kTestWord);
}

TEST(Trie, AddTwoWords) {
  Trie trie;
  constexpr char kTestWord1[] = "aaa";
  constexpr char kTestWord2[] = "aa";
  trie.Add(kTestWord1);
  trie.Add(kTestWord2);

  EXPECT_EQ(trie.GetLongestMatchingWord(kTestWord1), kTestWord1);
}

TEST(Trie, AddMany) {
  Trie trie;
  trie.Add("aab");
  trie.Add("aa");
  trie.Add("bbb");
  trie.Add("bb");

  EXPECT_EQ(trie.GetLongestMatchingWord(""), "");
  EXPECT_EQ(trie.GetLongestMatchingWord("aaab"), "aa");
  EXPECT_EQ(trie.GetLongestMatchingWord("aabaa"), "aab");
  EXPECT_EQ(trie.GetLongestMatchingWord("bbab"), "bb");
  EXPECT_EQ(trie.GetLongestMatchingWord("bbb"), "bbb");
}

}  // namespace text_splitter