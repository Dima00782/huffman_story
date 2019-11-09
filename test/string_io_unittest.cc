#include "gtest/gtest.h"
#include "string_io/string_byte_reader.h"
#include "string_io/string_byte_writer.h"

TEST(StringReader, Read) {
  const std::string kTestString = "Hello";
  string_io::StringByteReader string_byte_reader(kTestString);

  for (const auto symbol : kTestString) {
    const auto byte = string_byte_reader.ReadByte().value_or('\0');
    EXPECT_EQ(byte, symbol);
  }

  const auto byte = string_byte_reader.ReadByte();
  EXPECT_FALSE(byte.has_value());
}

TEST(StringWriter, Write) {
  const std::string kTestString = "Hello";
  string_io::StringByteWriter string_byte_writer;
  
  for (const auto symbol : kTestString) {
    string_byte_writer.WriteByte(symbol);
  }

  EXPECT_EQ(string_byte_writer.GetData(), kTestString);
}
