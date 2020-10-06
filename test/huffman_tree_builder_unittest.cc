#include <string>

#include "encryption/huffman_tree/huffman_tree_builder.h"
#include "encryption/text_splitter/text_splitter.h"
#include "gtest/gtest.h"

namespace huffman_tree {

bool IsInnerNode(TreeNode* node) {
  return node->left_ && node->right_;
}

bool IsLeafNode(TreeNode* node) {
  return !node->left_ && !node->right_;
}

class HuffmanTreeBuilderTest : public ::testing::Test {
 public:
  std::set<std::string> GetAllCharactersAlphabet() {
    std::set<std::string> alphabet;
    for (int letter = 0; letter < 256; ++letter) {
      alphabet.insert(std::string{static_cast<char>(letter)});
    }
    return alphabet;
  }

  std::unique_ptr<TreeNode> BuildTree(std::string_view text) {
    text_splitter::TextSplitter text_splitter{GetAllCharactersAlphabet()};
    return BuildHuffmanTree(text_splitter.Split(text));
  }
};

TEST_F(HuffmanTreeBuilderTest, OneSymbol) {
  const std::string kTestString = "aaaa";
  const auto root = BuildTree(kTestString);

  ASSERT_TRUE(root);
  EXPECT_TRUE(IsLeafNode(root.get()));
  EXPECT_EQ(root->key_, "a");
  EXPECT_EQ(root->frequency_, 4u);
}

TEST_F(HuffmanTreeBuilderTest, TwoSymbols) {
  const std::string kTestString = "aaaabbb";
  const auto root = BuildTree(kTestString);

  ASSERT_TRUE(root);
  EXPECT_TRUE(IsInnerNode(root.get()));
  EXPECT_EQ(root->frequency_, 7u);

  auto* left = root->left_.get();
  EXPECT_EQ(left->key_, "b");
  EXPECT_EQ(left->frequency_, 3u);
  EXPECT_TRUE(IsLeafNode(left));

  auto* right = root->right_.get();
  EXPECT_EQ(right->key_, "a");
  EXPECT_EQ(right->frequency_, 4u);
  EXPECT_TRUE(IsLeafNode(right));
}

TEST_F(HuffmanTreeBuilderTest, ThreeSymbols) {
  const std::string kTestString = "aaaabbc";
  const auto root = BuildTree(kTestString);

  ASSERT_TRUE(root);
  EXPECT_TRUE(IsInnerNode(root.get()));
  EXPECT_EQ(root->frequency_, 7u);

  auto* left = root->left_.get();
  EXPECT_TRUE(IsInnerNode(left));
  EXPECT_EQ(left->frequency_, 3u);

  auto* right = root->right_.get();
  EXPECT_EQ(right->key_, "a");
  EXPECT_EQ(right->frequency_, 4u);
  EXPECT_TRUE(IsLeafNode(right));

  auto* left_left = left->left_.get();
  EXPECT_TRUE(IsLeafNode(left_left));
  EXPECT_EQ(left_left->key_, "c");
  EXPECT_EQ(left_left->frequency_, 1u);

  auto* left_right = left->right_.get();
  EXPECT_TRUE(IsLeafNode(left_right));
  EXPECT_EQ(left_right->key_, "b");
  EXPECT_EQ(left_right->frequency_, 2u);
}

}  // namespace huffman_tree
