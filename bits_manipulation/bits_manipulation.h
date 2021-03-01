#ifndef BITS_MANIPULATION_BITS_MANIPULATION_H_
#define BITS_MANIPULATION_BITS_MANIPULATION_H_

#include <array>
#include <cassert>
#include <climits>
#include <cstddef>
#include <cstdint>

namespace bits_manipulation {

inline bool IsBitEnabled(std::byte byte, uint8_t pos) {
  return (byte & (std::byte{1} << (CHAR_BIT - pos - 1))) != std::byte{0};
}

inline std::byte SetBitInByte(std::byte byte, uint8_t pos, bool enabled) {
  if (enabled) {
    return byte | (std::byte{1} << (CHAR_BIT - pos - 1));
  }

  return byte & ~(std::byte{1} << (CHAR_BIT - pos - 1));
}

inline std::array<bool, CHAR_BIT> ByteToBits(std::byte byte) {
  std::array<bool, CHAR_BIT> bits;
  for (uint32_t bit_pos = 0; bit_pos < bits.size(); ++bit_pos) {
    bits[bit_pos] = IsBitEnabled(byte, bit_pos);
  }

  return bits;
}

template <std::size_t SizeInByte>
class FixedSizeBitSet {
 public:
  void PushBack(bool enabled) {
    assert(bit_idx_ / CHAR_BIT < SizeInByte);

    std::byte& byte = buffer_[bit_idx_ / CHAR_BIT];
    byte = SetBitInByte(byte, bit_idx_ % CHAR_BIT, enabled);
    ++bit_idx_;
  }

  uint32_t SizeInBits() const { return bit_idx_; }

  void Clear() { bit_idx_ = 0u; }

  const char* GetAsCharArray() const {
    return reinterpret_cast<const char*>(&buffer_[0]);
  }

 private:
  std::array<std::byte, SizeInByte> buffer_;
  uint32_t bit_idx_{0u};
};

}  // namespace bits_manipulation

#endif  // BITS_MANIPULATION_BITS_MANIPULATION_H_
