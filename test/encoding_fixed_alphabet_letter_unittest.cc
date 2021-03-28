#include "gtest/gtest.h"

#include <iterator>
#include <memory>
#include <set>
#include <sstream>
#include <string>

#include "bits_manipulation/bits_manipulation.h"
#include "encoding/byte_streams_adapters/byte_aligned_bit_reader.h"
#include "encoding/byte_streams_adapters/byte_aligned_bit_writer.h"
#include "encoding/huffman_encoding.h"
#include "letter/fixed_alphabet_letter/fixed_alphabet_letter.h"

namespace {

template <letter::LetterConfig Config>
std::string EncryptTextBase(const std::string& text,
                            std::shared_ptr<Config> config) {
  auto input = std::make_shared<std::istringstream>(text);
  auto output = std::make_shared<std::ostringstream>();
  encoding::HuffmanEncoder<Config>(std::move(config), input, output);
  return output->str();
}

template <letter::LetterConfig Config>
std::string DecryptTextBase(const std::string& text,
                            std::shared_ptr<Config> config) {
  auto input = std::make_shared<std::istringstream>(text);
  auto output = std::make_shared<std::ostringstream>();
  encoding::HuffmanDecoder<Config>(std::move(config), input, output);
  return output->str();
}

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
    return EncryptTextBase<ConfigType>(text, config_);
  }

  std::string Decrypt(const std::string& text) {
    return DecryptTextBase<ConfigType>(text, config_);
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
