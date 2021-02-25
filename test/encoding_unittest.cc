#include "gtest/gtest.h"

#include <iterator>
#include <memory>
#include <set>
#include <sstream>
#include <string>

#include "bits_manipulation/bits_manipulation.h"
#include "encoding/char_streams_adapters/char_aligned_bit_reader.h"
#include "encoding/char_streams_adapters/char_aligned_bit_writer.h"
#include "encoding/huffman_encoding.h"
#include "letter/one_byte_letter.h"

template <letter::LetterConfig Config>
std::string EncryptTextBase(const std::string& text,
                            std::shared_ptr<Config> config) {
  auto input = std::make_shared<std::istringstream>(text);
  auto output = std::make_shared<std::ostringstream>();
  encoding::HuffmanEncrypt<Config>(std::move(config), input, output);
  return output->str();
}

template <letter::LetterConfig Config>
std::string DecryptTextBase(const std::string& text,
                            std::shared_ptr<Config> config) {
  auto input = std::make_shared<std::istringstream>(text);
  auto output = std::make_shared<std::ostringstream>();
  encoding::HuffmanDecrypt<Config>(std::move(config), input, output);
  return output->str();
}

struct TestCase {
  std::string input;
  std::string expected_output;
};

class EncodingAcceptanceTestOneByteLetter
    : public ::testing::TestWithParam<TestCase> {
 public:
  void SetUp() override {
    config_ = std::make_shared<letter::OneByteLetterConfig>();
  }

  std::string Encrypt(const std::string& text) {
    return EncryptTextBase<letter::OneByteLetterConfig>(text, config_);
  }

  std::string Decrypt(const std::string& text) {
    return DecryptTextBase<letter::OneByteLetterConfig>(text, config_);
  }

  std::shared_ptr<letter::OneByteLetterConfig> config_;
};

TEST_P(EncodingAcceptanceTestOneByteLetter, EncryptAndDecrypt) {
  EXPECT_EQ(Encrypt(GetParam().input), GetParam().expected_output);
  EXPECT_EQ(Decrypt(GetParam().expected_output), GetParam().input);
}

INSTANTIATE_TEST_SUITE_P(
    AcceptanceTests,
    EncodingAcceptanceTestOneByteLetter,
    ::testing::Values(
        TestCase{"aaaaaaa",
                 std::string("\xb0\x80\x00", 3)},  // Zero unused bits.
        TestCase{"aaaaaa", std::string("\xb0\x80\x01", 3)},  // One unused bit.
        TestCase{"aaaaa", std::string("\xb0\x80\x02", 3)},   // Two unused bits.
        TestCase{"aaaa", std::string("\xb0\x83", 2)},  // Three unused bits.
        TestCase{"aaa", std::string("\xb0\x84", 2)},   // Four unused bits.
        TestCase{"aa", std::string("\xb0\x85", 2)},    // Five unused bits.
        TestCase{"a", std::string("\xb0\x86", 2)},     // Six unused bits.
        TestCase{"aaaaaaaa",
                 std::string("\xb0\x80\x07", 3)},  // Seven unused bits.
        TestCase{"aaaaa bbb", std::string("\x24\x16\x2b\x0f\xc5\x46", 6)},
        TestCase{"aaabb", std::string("\x58\xac\x3c\x00", 4)},
        TestCase{"aaaabbc", std::string("\x2c\x76\x2b\x0f\xa8\x01", 6)}));

/*
TODO: need to implement multiple byte letter
class EncodingAcceptanceTestCustomAlphabet
    : public EncodingAcceptanceTestBase {};

TEST_F(EncodingAcceptanceTestCustomAlphabet, OneLetterAlphabet) {
  SetAlphabet({"aa"});
  constexpr char kText[] = "aaaaaa";
  const std::string expected_output{"\x81\x30\xb0\x84", 4};
  EXPECT_EQ(EncryptText(kText), expected_output);
}
*/