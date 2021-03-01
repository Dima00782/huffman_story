#include "bits_manipulation/bits_manipulation.h"
#include "gtest/gtest.h"

TEST(BitManipulation, FixedSizeBitSet) {
  constexpr std::size_t kBitSetSizeInBytes = 1u;
  bits_manipulation::FixedSizeBitSet<kBitSetSizeInBytes> bitset;

  EXPECT_EQ(bitset.SizeInBits(), 0u);
  bitset.PushBack(true);
  EXPECT_EQ(bitset.SizeInBits(), 1u);
  bitset.PushBack(false);
  EXPECT_EQ(bitset.SizeInBits(), 2u);
  bitset.PushBack(true);
  EXPECT_EQ(bitset.SizeInBits(), 3u);
  bitset.PushBack(true);
  EXPECT_EQ(bitset.SizeInBits(), 4u);
  bitset.PushBack(false);
  EXPECT_EQ(bitset.SizeInBits(), 5u);
  bitset.PushBack(false);
  EXPECT_EQ(bitset.SizeInBits(), 6u);
  bitset.PushBack(false);
  EXPECT_EQ(bitset.SizeInBits(), 7u);
  bitset.PushBack(true);
  EXPECT_EQ(bitset.SizeInBits(), 8u);

  EXPECT_EQ(*bitset.GetAsCharArray(), '\xb1');
  bitset.Clear();
  EXPECT_EQ(bitset.SizeInBits(), 0u);
}
