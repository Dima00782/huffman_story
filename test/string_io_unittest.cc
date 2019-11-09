#include "gtest/gtest.h"
#include "string_io/string_byte_reader.h"

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
