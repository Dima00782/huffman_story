#ifndef BITS_MANIPULATION_BITS_MANIPULATION_H_
#define BITS_MANIPULATION_BITS_MANIPULATION_H_

#include <array>
#include <cstdint>
#include <climits>
#include <string>
#include <vector>

namespace bits_manipulation {

bool IsBitEnabled(char byte, uint8_t pos);
char SetBitInByte(char byte, uint8_t pos, bool enabled);
std::array<bool, CHAR_BIT> ByteToBits(char byte);

}  // namespace bits_manipulation

#endif  // BITS_MANIPULATION_BITS_MANIPULATION_H_
