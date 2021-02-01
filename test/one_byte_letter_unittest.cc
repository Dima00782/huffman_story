#include "letter/one_byte_letter.h"
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

    const bool result = bits_manipulation::IsBitEnabled(content_[byte_index],
                                                        bit_index_ % CHAR_BIT);
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

  void WriteBit(bool enabled) override {
    const uint32_t byte_index = bit_index_ / CHAR_BIT;
    if (byte_index >= content_.size()) {
      content_.push_back('\0');
    }
    content_[byte_index] = bits_manipulation::SetBitInByte(
        content_[byte_index], bit_index_ % CHAR_BIT, enabled);
    ++bit_index_;
  }

  const std::string& getContent() const { return content_; }

 private:
  uint32_t bit_index_{0};
  std::string content_;
};
}  // namespace

namespace letter {

TEST(OneByteLetter, Trivial) {
  auto letter = std::make_unique<OneByteLetter>(std::byte{'a'});
}

TEST(OneByteLetter, Hashable) {
  std::unordered_map<std::unique_ptr<Letter>, int> map;
  map.insert(std::pair(std::make_unique<OneByteLetter>(std::byte{'a'}), 10));
  map.insert(std::pair(std::make_unique<OneByteLetter>(std::byte{'b'}), 20));
  EXPECT_EQ(map[std::make_unique<OneByteLetter>(std::byte{'a'})], 10);
  EXPECT_EQ(map[std::make_unique<OneByteLetter>(std::byte{'a'})], 10);
  EXPECT_EQ(map[std::make_unique<OneByteLetter>(std::byte{'b'})], 20);
  EXPECT_EQ(map[std::make_unique<OneByteLetter>(std::byte{'b'})], 20);
}

TEST(OneByteLetter, Lexer) {
  auto lexer = std::make_unique<OneByteLetterLexer>();
  auto result = lexer->Split(std::make_shared<std::istringstream>("abcdefg"));
  EXPECT_EQ(result.size(), 7u);
  EXPECT_EQ(result[0]->toString(), "a");
  EXPECT_EQ(result[1]->toString(), "b");
  EXPECT_EQ(result[2]->toString(), "c");
  EXPECT_EQ(result[3]->toString(), "d");
  EXPECT_EQ(result[4]->toString(), "e");
  EXPECT_EQ(result[5]->toString(), "f");
  EXPECT_EQ(result[6]->toString(), "g");
}

TEST(OneByteLetter, SerializerRead) {
  auto serializer = std::make_unique<OneByteLetterSerializer>();
  StringBasedBitReader input("abc");
  EXPECT_EQ(serializer->ReadSerialized(input)->toString(), "a");
  EXPECT_EQ(serializer->ReadSerialized(input)->toString(), "b");
  EXPECT_EQ(serializer->ReadSerialized(input)->toString(), "c");
  EXPECT_EQ(serializer->ReadSerialized(input), nullptr);
}

TEST(OneByteLetter, SerializerWrite) {
  auto serializer = std::make_unique<OneByteLetterSerializer>();
  StringBasedBitWriter output;
  ASSERT_TRUE(
      serializer->WriteSerialized(output, OneByteLetter(std::byte{'a'})));
  ASSERT_TRUE(
      serializer->WriteSerialized(output, OneByteLetter(std::byte{'b'})));
  ASSERT_TRUE(
      serializer->WriteSerialized(output, OneByteLetter(std::byte{'c'})));
  EXPECT_EQ(output.getContent(), "abc");
}

}  // namespace letter