#include "bits_manipulation/bits_manipulation.h"
#include "encryption/char_streams_adapters/char_istream_adapter.h"
#include "encryption/char_streams_adapters/char_ostream_adapter.h"
#include "encryption/huffman_encryption.h"
#include "gtest/gtest.h"

#include <iterator>
#include <set>
#include <sstream>
#include <string>

/*
class EncryptionAcceptanceTestBase : public ::testing::Test {
 public:
  std::string EncryptText(const std::string& text) {
    auto string_input = std::make_shared<std::istringstream>(text);
    auto ostring_stream = std::make_shared<std::ostringstream>();
    encryption::HuffmanEncrypt(string_input, ostring_stream, alphabet_);
    return ostring_stream->str();
  }

  std::string DecryptText(const std::string& text) {
    auto string_input = std::make_shared<std::istringstream>(text);
    auto string_output = std::make_shared<std::ostringstream>();
    encryption::HuffmanDecrypt(string_input, string_output);
    return string_output->str();
  }

  void SetAlphabet(std::set<std::string> alphabet) {
    alphabet_ = std::move(alphabet);
  }

 private:
  std::set<std::string> alphabet_;
};

struct TestCase {
  std::string input;
  std::string expected_output;
};

std::set<std::string> GetEnglishAlphabet() {
  std::set<std::string> english_alphabet;
  for (char letter = 'a'; letter <= 'z'; ++letter) {
    english_alphabet.insert(std::string{letter});
  }
  return english_alphabet;
}

class EncryptionAcceptanceTestEnglishAlphabet
    : public EncryptionAcceptanceTestBase,
      public ::testing::WithParamInterface<TestCase> {
 public:
  EncryptionAcceptanceTestEnglishAlphabet() {
    SetAlphabet(GetEnglishAlphabet());
  }
};

TEST_P(EncryptionAcceptanceTestEnglishAlphabet, EncryptAndDecrypt) {
  EXPECT_EQ(EncryptText(GetParam().input), GetParam().expected_output);
  EXPECT_EQ(DecryptText(GetParam().expected_output), GetParam().input);
}

INSTANTIATE_TEST_CASE_P(
    AcceptanceTests,
    EncryptionAcceptanceTestEnglishAlphabet,
    ::testing::Values(
        TestCase{"aaaaaaa",
                 std::string("\x80\xb0\x80\x00", 4)},  // Zero unused bits.
        TestCase{"aaaaaa",
                 std::string("\x80\xb0\x80\x01", 4)},  // One unused bit.
        TestCase{"aaaaa",
                 std::string("\x80\xb0\x80\x02", 4)},      // Two unused bits.
        TestCase{"aaaa", std::string("\x80\xb0\x83", 3)},  // Three unused bits.
        TestCase{"aaa", std::string("\x80\xb0\x84", 3)},   // Four unused bits.
        TestCase{"aa", std::string("\x80\xb0\x85", 3)},    // Five unused bits.
        TestCase{"a", std::string("\x80\xb0\x86", 3)},     // Six unused bits.
        TestCase{"aaaaaaaa",
                 std::string("\x80\xb0\x80\x07", 4)},  // Seven unused bits.
        TestCase{"aaabb", std::string("\x40\x58\xa0\x2c\x3c\x00", 6)},
        TestCase{"aaaabbc",
                 std::string("\x20\x2c\x70\x16\x28\x0b\x0f\xa8\x01", 9)}));

class EncryptionAcceptanceTestCustomAlphabet
    : public EncryptionAcceptanceTestBase {};

TEST_F(EncryptionAcceptanceTestCustomAlphabet, OneLetterAlphabet) {
  SetAlphabet({"aa"});
  constexpr char kText[] = "aaaaaa";
  const std::string expected_output{"\x81\x30\xb0\x84", 4};
  EXPECT_EQ(EncryptText(kText), expected_output);
}
*/