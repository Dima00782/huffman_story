#ifndef BITS_MANIPULATION_BITS_MANIPULATION_H_
#define BITS_MANIPULATION_BITS_MANIPULATION_H_

#include <array>
#include <climits>
#include <cstddef>
#include <cstdint>

namespace bits_manipulation {

bool IsBitEnabled(std::byte byte, uint8_t pos);
std::byte SetBitInByte(std::byte byte, uint8_t pos, bool enabled);
std::array<bool, CHAR_BIT> ByteToBits(std::byte byte);

}  // namespace bits_manipulation

#endif  // BITS_MANIPULATION_BITS_MANIPULATION_H_
