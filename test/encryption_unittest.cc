#include "encryption/huffman_encryption.h"
#include "gtest/gtest.h"
#include "string_io/string_byte_reader.h"
#include "string_io/string_byte_writer.h"

TEST(Encryption, Trivial) {
  const std::string kTestInputString = "aaaabbc";
  const std::string kTestOutputTreePart = "001c1b1a";
  const std::string kTestOutputTextPart = "1111010100";

  auto string_reader = std::make_unique<string_io::StringByteReader>(kTestInputString);
  auto string_writer = std::make_unique<string_io::StringByteWriter>();
  auto* string_writer_ptr = string_writer.get();
  encryption::HuffmanEncryption huffman_encryption(std::move(string_reader), std::move(string_writer));

  EXPECT_EQ(string_writer_ptr->GetData(), kTestOutputTreePart + kTestOutputTextPart);
}
