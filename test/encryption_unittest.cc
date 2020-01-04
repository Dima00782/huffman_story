#include "bits_manipulation/bits_manipulation.h"
#include "char_streams_adapters/char_istream_adapter.h"
#include "char_streams_adapters/char_ostream_adapter.h"
#include "encryption/huffman_encryption.h"
#include "gtest/gtest.h"
#include "string_io/string_bit_reader.h"
#include "string_io/string_bit_writer.h"

#include <iterator>
#include <set>
#include <sstream>
#include <string>

struct TestCase {
  std::string input;
  std::string expected_output;
};

class EncryptionAcceptanceTests : public ::testing::TestWithParam<TestCase> {
 public:
  EncryptionAcceptanceTests() : english_alphabet_{GetEnglishAlphabet()} {}

  std::string EncryptText(const std::string& text) {
    auto string_input = std::make_shared<std::istringstream>(text);
    auto ostring_stream = std::make_shared<std::ostringstream>();
    {
      auto string_output =
          std::make_shared<char_adapters::CharOStreamAdapter>(ostring_stream);
      encryption::HuffmanEncrypt(string_input, string_output,
                                 english_alphabet_);
    }
    return ostring_stream->str();
  }

  std::string DecryptText(const std::string& text) {
    auto string_input = std::make_shared<char_adapters::CharIStreamAdapter>(
        std::make_shared<std::istringstream>(text));
    auto string_output = std::make_shared<std::ostringstream>();
    encryption::HuffmanDecrypt(string_input, string_output);
    return string_output->str();
  }

  static std::set<std::string> GetEnglishAlphabet() {
    std::set<std::string> english_alphabet;
    for (char letter = 'a'; letter <= 'z'; ++letter) {
      english_alphabet.insert(std::string{letter});
    }
    return english_alphabet;
  }

  const std::set<std::string> english_alphabet_;
};

TEST_P(EncryptionAcceptanceTests, EncryptAndDecrypt) {
  EXPECT_EQ(EncryptText(GetParam().input), GetParam().expected_output);
  EXPECT_EQ(DecryptText(GetParam().expected_output), GetParam().input);
}

constexpr char kTestOutput[] = {'\xb0', '\x86'};

INSTANTIATE_TEST_CASE_P(
    AcceptanceTests,
    EncryptionAcceptanceTests,
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
        TestCase{"aaabb", std::string("\x58\xac\x3c\x00", 4)},
        TestCase{"aaaabbc", std::string("\x2c\x76\x2b\x0f\xa8\x01", 6)}));
