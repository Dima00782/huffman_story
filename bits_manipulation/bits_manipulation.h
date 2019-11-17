#ifndef BITS_MANIPULATION_BITS_MANIPULATION_H_
#define BITS_MANIPULATION_BITS_MANIPULATION_H_

#include <cstdint>
#include <string>
#include <vector>

namespace bits_manipulation {

bool IsBitEnabled(char byte, uint8_t pos);
char SetBitInByte(char byte, uint8_t pos, bool enabled);
std::vector<bool> ByteToBits(char byte);
std::string InBinaryForm(uint16_t number);

}  // namespace bits_manipulation

#endif  // BITS_MANIPULATION_BITS_MANIPULATION_H_
