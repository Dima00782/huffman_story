#include "gtest/gtest.h"

#include <memory>
#include <set>
#include <sstream>
#include <string>

#include "letter/fixed_alphabet_letter/fixed_alphabet_letter.h"
#include "test/encoding_test_base.h"

namespace {

struct TestCase {
  std::set<std::string> alphabet;
  std::string input;
  std::string expected_output;
};

class EncodingAcceptanceTestFixedAlphabetLetter
    : public ::testing::TestWithParam<TestCase> {
 public:
  using ConfigType = fixed_alpha_letter::FixedAlphabetLetterConfig;

  void SetAlphabet(const std::set<std::string>& alphabet) {
    config_ = std::make_shared<ConfigType>(alphabet);
  }

  std::string Encrypt(const std::string& text) {
    return test::EncryptTextBase<ConfigType>(text, config_);
  }

  std::string Decrypt(const std::string& text) {
    return test::DecryptTextBase<ConfigType>(text, config_);
  }

  std::shared_ptr<ConfigType> config_;
};

TEST_P(EncodingAcceptanceTestFixedAlphabetLetter, EncryptAndDecrypt) {
  SetAlphabet(GetParam().alphabet);
  EXPECT_EQ(Encrypt(GetParam().input), GetParam().expected_output);
  EXPECT_EQ(Decrypt(GetParam().expected_output), GetParam().input);
}

INSTANTIATE_TEST_SUITE_P(
    AcceptanceTests,
    EncodingAcceptanceTestFixedAlphabetLetter,
    ::testing::Values(
        TestCase{{"aa"}, "aaaaaa", std::string("\x81\x30\xb0\x84", 4)},
        TestCase{{"aa"}, "aaaaaaaaaaaa", std::string("\x81\x30\xb0\x80\x01", 5)},
        TestCase{{"aa", "bb", " "}, " aa bb aa ", std::string("\x20\x4c\x4c\x50\x26\x16\x18\x09\x05\x96\x01", 11)}));

}  // namespace
