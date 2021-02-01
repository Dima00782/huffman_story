#include "gtest/gtest.h"

#include <iterator>
#include <set>
#include <sstream>
#include <string>

#include "bits_manipulation/bits_manipulation.h"
#include "encryption/char_streams_adapters/char_aligned_bit_reader.h"
#include "encryption/char_streams_adapters/char_aligned_bit_writer.h"
#include "encryption/huffman_encryption.h"
#include "letter/one_byte_letter.h"

// TODO: add a generic SetLexer and SetSerializer
class EncryptionAcceptanceTestBase : public ::testing::Test {
 public:
  std::string EncryptText(const std::string& text) {
    auto string_input = std::make_shared<std::istringstream>(text);
    auto ostring_stream = std::make_shared<std::ostringstream>();
    auto letterLexer = std::make_shared<letter::OneByteLetterLexer>();
    auto letter_serializer =
        std::make_shared<letter::OneByteLetterSerializer>();
    encryption::HuffmanEncrypt(string_input, ostring_stream, letterLexer,
                               letter_serializer);
    return ostring_stream->str();
  }

  std::string DecryptText(const std::string& text) {
    auto string_input = std::make_shared<std::istringstream>(text);
    auto string_output = std::make_shared<std::ostringstream>();
    auto letter_serializer =
        std::make_shared<letter::OneByteLetterSerializer>();
    encryption::HuffmanDecrypt(string_input, string_output, letter_serializer);
    return string_output->str();
  }
};

// TODO: add one byte letter into the name;
struct TestCase {
  std::string input;
  std::string expected_output;
};

class EncryptionAcceptanceTestOneByteLetter
    : public EncryptionAcceptanceTestBase,
      public ::testing::WithParamInterface<TestCase> {};

TEST_P(EncryptionAcceptanceTestOneByteLetter, EncryptAndDecrypt) {
  EXPECT_EQ(EncryptText(GetParam().input), GetParam().expected_output);
  EXPECT_EQ(DecryptText(GetParam().expected_output), GetParam().input);
}

INSTANTIATE_TEST_SUITE_P(
    AcceptanceTests,
    EncryptionAcceptanceTestOneByteLetter,
    ::testing::Values(
        TestCase{"aaaaaaa",
                 std::string("\xb0\x80\x00", 3)},  // Zero unused bits.
        TestCase{"aaaaaa",
                 std::string("\xb0\x80\x01", 3)},  // One unused bit.
        TestCase{"aaaaa",
                 std::string("\xb0\x80\x02", 3)},      // Two unused bits.
        TestCase{"aaaa", std::string("\xb0\x83", 2)},  // Three unused bits.
        TestCase{"aaa", std::string("\xb0\x84", 2)},   // Four unused bits.
        TestCase{"aa", std::string("\xb0\x85", 2)},    // Five unused bits.
        TestCase{"a", std::string("\xb0\x86", 2)},     // Six unused bits.
        TestCase{"aaaaaaaa",
                 std::string("\xb0\x80\x07", 3)},  // Seven unused bits.
        TestCase{"aaabb", std::string("\x58\xac\x3c\x00", 4)},
        TestCase{"aaaabbc",
                 std::string("\x2c\x76\x2b\x0f\xa8\x01", 6)}));


/*
TODO: need to implement multiple byte letter
class EncryptionAcceptanceTestCustomAlphabet
    : public EncryptionAcceptanceTestBase {};

TEST_F(EncryptionAcceptanceTestCustomAlphabet, OneLetterAlphabet) {
  SetAlphabet({"aa"});
  constexpr char kText[] = "aaaaaa";
  const std::string expected_output{"\x81\x30\xb0\x84", 4};
  EXPECT_EQ(EncryptText(kText), expected_output);
}
*/