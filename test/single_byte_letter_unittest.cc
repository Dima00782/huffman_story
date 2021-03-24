#include "letter/single_byte_letter/single_byte_letter.h"
#include "bits_manipulation/bits_manipulation.h"
#include "gtest/gtest.h"

#include <climits>
#include <memory>
#include <sstream>
#include <unordered_map>
#include <utility>

namespace {
class StringBasedBitReader : public bit_io::BitReader {
 public:
  explicit StringBasedBitReader(const std::string& content)
      : content_(content) {}
  ~StringBasedBitReader() override = default;

  std::optional<bool> ReadBit() override {
    const uint32_t byte_index = bit_index_ / CHAR_BIT;
    if (byte_index >= content_.size()) {
      return std::nullopt;
    }

    const bool result = bits_manipulation::IsBitEnabled(
        static_cast<std::byte>(content_[byte_index]), bit_index_ % CHAR_BIT);
    ++bit_index_;
    return result;
  }

 private:
  uint32_t bit_index_{0};
  std::string content_;
};

class StringBasedBitWriter : public bit_io::BitWriter {
 public:
  ~StringBasedBitWriter() override = default;

  bool WriteBit(bool enabled) override {
    const uint32_t byte_index = bit_index_ / CHAR_BIT;
    if (byte_index >= content_.size()) {
      content_.push_back('\0');
    }
    content_[byte_index] = static_cast<char>(bits_manipulation::SetBitInByte(
        static_cast<std::byte>(content_[byte_index]), bit_index_ % CHAR_BIT,
        enabled));
    ++bit_index_;

    return true;
  }

  bool WriteFooter() override { return true; }

  const std::string& getContent() const { return content_; }

 private:
  uint32_t bit_index_{0};
  std::string content_;
};
}  // namespace

namespace single_byte_letter {

TEST(SingleByteLetter, Parser) {
  using Parser = SingleByteLetterConfig::LetterParser;

  auto input = std::make_shared<std::istringstream>(" abc defg ");
  Parser parser(std::move(input));
  EXPECT_EQ(*parser.Parse(), std::byte(' '));
  EXPECT_EQ(*parser.Parse(), std::byte('a'));
  EXPECT_EQ(*parser.Parse(), std::byte('b'));
  EXPECT_EQ(*parser.Parse(), std::byte('c'));
  EXPECT_EQ(*parser.Parse(), std::byte(' '));
  EXPECT_EQ(*parser.Parse(), std::byte('d'));
  EXPECT_EQ(*parser.Parse(), std::byte('e'));
  EXPECT_EQ(*parser.Parse(), std::byte('f'));
  EXPECT_EQ(*parser.Parse(), std::byte('g'));
  EXPECT_EQ(*parser.Parse(), std::byte(' '));
  EXPECT_FALSE(parser.Parse());
}

TEST(SingleByteLetter, SerializerRead) {
  SingleByteLetterConfig config;
  StringBasedBitReader input("abc");
  EXPECT_EQ(*config.ReadSerialized(input), std::byte('a'));
  EXPECT_EQ(*config.ReadSerialized(input), std::byte('b'));
  EXPECT_EQ(*config.ReadSerialized(input), std::byte('c'));
  EXPECT_FALSE(config.ReadSerialized(input));
}

TEST(SingleByteLetter, SerializerWrite) {
  SingleByteLetterConfig config;
  StringBasedBitWriter output;
  ASSERT_TRUE(config.WriteSerialized(output, std::byte{'a'}));
  ASSERT_TRUE(config.WriteSerialized(output, std::byte{'b'}));
  ASSERT_TRUE(config.WriteSerialized(output, std::byte{'c'}));
  EXPECT_EQ(output.getContent(), "abc");
}

}  // namespace single_byte_letter