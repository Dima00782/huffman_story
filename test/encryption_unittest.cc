#include "bits_manipulation/bits_manipulation.h"
#include "encryption/huffman_encryption.h"
#include "gtest/gtest.h"
#include "string_io/string_bit_reader.h"
#include "string_io/string_bit_writer.h"

std::string EncryptText(const std::string& text) {
  auto string_input = std::make_unique<string_io::StringBitReader>(text);
  auto string_output = std::make_unique<string_io::StringBitWriter>();

  auto* string_output_ptr = string_output.get();
  encryption::HuffmanEncryption huffman;
  huffman.Encrypt(std::move(string_input), std::move(string_output));
  return string_output_ptr->GetData();
}

std::string DecryptText(const std::string& text) {
  auto string_input = std::make_unique<string_io::StringBitReader>(text);
  auto string_output = std::make_unique<string_io::StringBitWriter>();

  auto* string_output_ptr = string_output.get();
  encryption::HuffmanEncryption huffman;
  huffman.Decrypt(std::move(string_input), std::move(string_output));
  return string_output_ptr->GetData();
}

TEST(Encryption, NoSymbols) {
  constexpr char kTestInputString[] = "";
  constexpr char kTestOutput[] = "";

  EXPECT_EQ(EncryptText(kTestInputString), kTestOutput);
  EXPECT_EQ(DecryptText(kTestOutput), kTestInputString);
}

TEST(Encryption, OneSymbol) {
  constexpr char kTestInputString[] = "a";
  const auto expected_output =
      bits_manipulation::TwoByteNumberAsTwoCharString(0b1'01100001'0'110'000);

  EXPECT_EQ(EncryptText(kTestInputString), expected_output);
}

TEST(Encryption, TwoSymbols) {
  constexpr char kTestInputString[] = "aaabb";
  const auto expected_output =
      bits_manipulation::TwoByteNumberAsTwoCharString(0b01'01100010'1'01100) +
      bits_manipulation::TwoByteNumberAsTwoCharString(0b001'11100'000'00000);

  EXPECT_EQ(EncryptText(kTestInputString), expected_output);
}

TEST(Encryption, ThreeSymbols) {
  constexpr char kTestInputString[] = "aaaabbc";
  const auto expected_output =
      bits_manipulation::TwoByteNumberAsTwoCharString(0b001'01100011'1'0110) +
      bits_manipulation::TwoByteNumberAsTwoCharString(0b0010'1'01100001'111) +
      bits_manipulation::TwoByteNumberAsTwoCharString(0b1010100'001'000000);

  EXPECT_EQ(EncryptText(kTestInputString), expected_output);
}