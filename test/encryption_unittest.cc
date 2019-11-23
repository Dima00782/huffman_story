#include "bits_manipulation/bits_manipulation.h"
#include "encryption/huffman_encryption.h"
#include "gtest/gtest.h"
#include "string_io/string_bit_writer.h"
#include "string_io/string_byte_reader.h"

std::string CompressText(const std::string& text) {
  auto string_reader = std::make_unique<string_io::StringByteReader>(text);
  auto string_writer = std::make_unique<string_io::StringBitWriter>();
  auto* string_writer_ptr = string_writer.get();
  encryption::HuffmanEncryption huffman_encryption;
  huffman_encryption.Encrypt(std::move(string_reader),
                             std::move(string_writer));
  return string_writer_ptr->GetData();
}

TEST(Encryption, NoSymbols) {
  const std::string kTestInputString = "";
  const std::string kTestOutput = "";

  EXPECT_EQ(CompressText(kTestInputString), kTestOutput);
}

TEST(Encryption, OneSymbol) {
  const std::string kTestInputString = "a";
  EXPECT_EQ(CompressText(kTestInputString),
            bits_manipulation::InBinaryForm(0b1'01100001'0000000));
}

TEST(Encryption, TwoSymbols) {
  const std::string kTestInputString = "aaabb";

  EXPECT_EQ(CompressText(kTestInputString),
            bits_manipulation::InBinaryForm(0b01'01100010'1'01100) +
                static_cast<char>(0b001'11100));
}

TEST(Encryption, ThreeSymbols) {
  const std::string kTestInputString = "aaaabbc";

  EXPECT_EQ(CompressText(kTestInputString),
            bits_manipulation::InBinaryForm(0b001'01100011'1'0110) +
                bits_manipulation::InBinaryForm(0b0010'1'01100001'111) +
                static_cast<char>(0b10101000));
}
