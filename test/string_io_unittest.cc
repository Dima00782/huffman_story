#include "bits_manipulation/bits_manipulation.h"
#include "gtest/gtest.h"
#include "string_io/string_bit_reader.h"
#include "string_io/string_bit_writer.h"

#include <climits>
#include <cstdint>

TEST(StringReader, ReadBytes) {
  const std::string kTestString = "Hello";
  string_io::StringBitReader bit_reader(kTestString);

  for (const auto symbol : kTestString) {
    const auto byte = bit_reader.ReadByte().value_or('\0');
    EXPECT_EQ(byte, symbol);
  }

  const auto byte = bit_reader.ReadByte();
  EXPECT_FALSE(byte.has_value());
}

TEST(StringReader, ReadBits) {
  const std::string kTestString = "\xab";
  string_io::StringBitReader bit_reader(kTestString);

  const auto first_bit = bit_reader.ReadBit();
  const auto second_bit = bit_reader.ReadBit();
  const auto third_bit = bit_reader.ReadBit();
  const auto fourth_bit = bit_reader.ReadBit();
  EXPECT_TRUE(first_bit.has_value() && second_bit.has_value() &&
              third_bit.has_value() && fourth_bit.has_value());
  EXPECT_TRUE(*first_bit && !*second_bit && *third_bit && !*fourth_bit);

  const auto fifth_bit = bit_reader.ReadBit();
  const auto sixth_bit = bit_reader.ReadBit();
  const auto seventh_bit = bit_reader.ReadBit();
  const auto eighth_bit = bit_reader.ReadBit();
  EXPECT_TRUE(fifth_bit.has_value() && sixth_bit.has_value() &&
              seventh_bit.has_value() && eighth_bit.has_value());
  EXPECT_TRUE(*fifth_bit && !*sixth_bit && *seventh_bit && *eighth_bit);

  const auto other_bit = bit_reader.ReadBit();
  EXPECT_FALSE(other_bit.has_value());
}

TEST(StringWriter, WriteBytes) {
  const std::string kTestString = "Hello";
  std::string buffer;
  {
    string_io::StringBitWriter bit_writer(&buffer);

    for (const auto symbol : kTestString) {
      bit_writer.WriteByte(symbol);
    }
  }

  EXPECT_EQ(buffer, kTestString);
}

TEST(StringReader, WriteBitsFullByte) {
  const std::string kTestString = "\xab";
  std::string buffer;
  {
    string_io::StringBitWriter bit_writer(&buffer);

    for (const auto symbol : kTestString) {
      for (uint8_t bit_pos = 0; bit_pos < CHAR_BIT; ++bit_pos) {
        bit_writer.WriteBit(bits_manipulation::IsBitEnabled(symbol, bit_pos));
      }
    }
  }

  EXPECT_EQ(buffer, kTestString);
}

TEST(StringReader, WriteBitsNotFullByte) {
  const std::string kTestString = "\xa0";
  std::string buffer;
  {
    string_io::StringBitWriter bit_writer(&buffer);

    bit_writer.WriteBit(true);
    bit_writer.WriteBit(false);
    bit_writer.WriteBit(true);
  }
  EXPECT_EQ(buffer, kTestString);
}
