#include "encryption/huffman_encryption.h"
#include "gtest/gtest.h"
#include "string_io/string_byte_reader.h"
#include "string_io/string_byte_writer.h"

std::string CompressText(const std::string& text) {
  auto string_reader = std::make_unique<string_io::StringByteReader>(text);
  auto string_writer = std::make_unique<string_io::StringByteWriter>();
  auto* string_writer_ptr = string_writer.get();
  encryption::HuffmanEncryption huffman_encryption(std::move(string_reader),
                                                   std::move(string_writer));
  return string_writer_ptr->GetData();
}

TEST(Encryption, NoSymbols) {
  const std::string kTestInputString = "";
  const std::string kTestOutputTreePart = "";
  const std::string kTestOutputTextPart = "";

  EXPECT_EQ(CompressText(kTestInputString),
            kTestOutputTreePart + kTestOutputTextPart);
}

TEST(Encryption, OneSymbol) {
  const std::string kTestInputString = "a";
  const std::string kTestOutputTreePart = "1a";
  const std::string kTestOutputTextPart = "0";

  EXPECT_EQ(CompressText(kTestInputString),
            kTestOutputTreePart + kTestOutputTextPart);
}

TEST(Encryption, TwoSymbols) {
  const std::string kTestInputString = "aaabb";
  const std::string kTestOutputTreePart = "01b1a";
  const std::string kTestOutputTextPart = "11100";

  EXPECT_EQ(CompressText(kTestInputString),
            kTestOutputTreePart + kTestOutputTextPart);
}

TEST(Encryption, ThreeSymbols) {
  const std::string kTestInputString = "aaaabbc";
  const std::string kTestOutputTreePart = "001c1b1a";
  const std::string kTestOutputTextPart = "1111010100";

  EXPECT_EQ(CompressText(kTestInputString),
            kTestOutputTreePart + kTestOutputTextPart);
}
