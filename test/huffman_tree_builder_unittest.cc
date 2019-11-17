#include <string>

#include "encryption/huffman_tree/huffman_tree_builder.h"
#include "gtest/gtest.h"

bool IsInnerNode(encryption::TreeNode* node) {
  return node->left_ && node->right_;
}

bool IsLeafNode(encryption::TreeNode* node) {
  return !node->left_ && !node->right_;
}

TEST(HuffmanTreeBuilder, OneSymbol) {
  const std::string kTestString = "aaaa";
  const auto root = encryption::HuffmanTreeBuilder(kTestString).GetRoot();

  ASSERT_TRUE(root);
  EXPECT_TRUE(IsLeafNode(root.get()));
  // EXPECT_EQ(root->key_, "a");
  EXPECT_EQ(root->frequency_, 4u);
}

TEST(HuffmanTreeBuilder, TwoSymbols) {
  const std::string kTestString = "aaaabbb";
  const auto root = encryption::HuffmanTreeBuilder(kTestString).GetRoot();

  ASSERT_TRUE(root);
  EXPECT_TRUE(IsInnerNode(root.get()));
  EXPECT_EQ(root->frequency_, 7u);

  auto* left = root->left_.get();
  // EXPECT_EQ(left->key_, "b");
  EXPECT_EQ(left->frequency_, 3u);
  EXPECT_TRUE(IsLeafNode(left));

  auto* right = root->right_.get();
  // EXPECT_EQ(right->key_, "a");
  EXPECT_EQ(right->frequency_, 4u);
  EXPECT_TRUE(IsLeafNode(right));
}

TEST(HuffmanTreeBuilder, ThreeSymbols) {
  const std::string kTestString = "aaaabbc";
  const auto root = encryption::HuffmanTreeBuilder(kTestString).GetRoot();

  ASSERT_TRUE(root);
  EXPECT_TRUE(IsInnerNode(root.get()));
  EXPECT_EQ(root->frequency_, 7u);

  auto* left = root->left_.get();
  EXPECT_TRUE(IsInnerNode(left));
  EXPECT_EQ(left->frequency_, 3u);

  auto* right = root->right_.get();
  // EXPECT_EQ(right->key_, "a");
  EXPECT_EQ(right->frequency_, 4u);
  EXPECT_TRUE(IsLeafNode(right));

  auto* left_left = left->left_.get();
  EXPECT_TRUE(IsLeafNode(left_left));
  // EXPECT_EQ(left_left->key_, "c");
  EXPECT_EQ(left_left->frequency_, 1u);

  auto* left_right = left->right_.get();
  EXPECT_TRUE(IsLeafNode(left_right));
  // EXPECT_EQ(left_right->key_, "b");
  EXPECT_EQ(left_right->frequency_, 2u);
}
