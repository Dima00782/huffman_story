#ifndef LETTER_FIXED_ALPHABET_LETTER_H_
#define LETTER_FIXED_ALPHABET_LETTER_H_

#include <algorithm>
#include <array>
#include <cassert>
#include <climits>
#include <istream>
#include <memory>
#include <set>

#include "bit_io/bit_reader.h"
#include "bit_io/bit_writer.h"
#include "letter/fixed_alphabet_letter/trie.h"

namespace {
constexpr std::size_t kParserBufferSizeInBytes = 1024;
constexpr std::size_t kMaxLetterSize = 255;
}  // namespace

namespace fixed_alpha_letter {

class FixedAlphabetLetterParser final {
 public:
  explicit FixedAlphabetLetterParser(std::set<std::string> alphabet,
                                     std::shared_ptr<std::istream> input)
      : input_(std::move(input)) {
    for (const auto& letter : alphabet) {
      trie_.Add(letter);
    }
    assert(kParserBufferSizeInBytes > trie_.MaxWordLenght());
  }

  std::optional<std::string> Parse() {
    if (!has_input_ended_ && BufferSize() < trie_.MaxWordLenght()) {
      if (!FillBuffer()) {
        return std::nullopt;
      }
    }

    if (IsBufferEmpty()) {
      return std::nullopt;
    }

    auto max = trie_.GetLongestMatchingWord(&buffer_[buffer_start_pos_]);
    if (max.empty()) {
      return std::string(1u, buffer_[buffer_start_pos_++]);
    }
    buffer_start_pos_ += max.size();
    return max;
  }

  bool HasNext() const { return input_->rdbuf()->in_avail() > 0; }

 private:
  std::size_t BufferSize() const { return buffer_end_pos_ - buffer_start_pos_; }
  std::size_t IsBufferEmpty() const { return BufferSize() == 0u; }

  bool FillBuffer() {
    if (has_input_ended_) {
      // Nothing to do here.
      return false;
    }

    std::copy(std::begin(buffer_) + buffer_start_pos_,
              std::begin(buffer_) + buffer_end_pos_, std::begin(buffer_));
    buffer_end_pos_ = BufferSize();
    buffer_start_pos_ = 0u;

    const std::size_t num_bytes_to_read =
        kParserBufferSizeInBytes - BufferSize();
    input_->read(&buffer_[0] + buffer_end_pos_, num_bytes_to_read);
    const std::size_t num_bytes_read = input_->gcount();
    if (num_bytes_read == 0 || input_->bad()) {
      return false;
    }

    if (input_->fail() || num_bytes_read != num_bytes_to_read) {
      has_input_ended_ = true;
    }

    buffer_end_pos_ += num_bytes_read;
    return true;
  }

  Trie trie_;
  bool has_input_ended_{false};
  std::array<char, kParserBufferSizeInBytes> buffer_;
  std::size_t buffer_start_pos_{0};
  std::size_t buffer_end_pos_{0};
  std::shared_ptr<std::istream> input_;
};  // namespace fixed_alpha_letter

class FixedAlphabetLetterConfig {
 public:
  using LetterType = std::string;
  using LetterParser = FixedAlphabetLetterParser;

  FixedAlphabetLetterConfig(std::set<std::string> alphabet)
      : alphabet_(std::move(alphabet)) {}

  std::unique_ptr<LetterParser> CreateParser(
      std::shared_ptr<std::istream> input) {
    return std::make_unique<LetterParser>(alphabet_, std::move(input));
  }

  bool Write(std::ostream& output, LetterType letter) {
    return static_cast<bool>(output << letter);
  }

  bool WriteSerialized(bit_io::BitWriter& bit_output, LetterType letter) {
    assert(letter.size() < kMaxLetterSize);
    const auto size = static_cast<std::byte>(letter.size());
    if (!bit_output.WriteByte(size)) {
      return false;
    }
    for (const auto symbol : letter) {
      if (!bit_output.WriteByte(static_cast<std::byte>(symbol))) {
        return false;
      }
    }
    return true;
  }

  std::optional<LetterType> ReadSerialized(bit_io::BitReader& bit_reader) {
    auto size_byte = bit_reader.ReadByte();
    if (!size_byte) {
      return std::nullopt;
    }

    const auto result_size = std::to_integer<std::size_t>(*size_byte);
    LetterType letter(result_size, '\0');
    for (std::size_t num_byte = 0; num_byte < result_size; ++num_byte) {
      auto byte = bit_reader.ReadByte();
      if (!byte) {
        return std::nullopt;
      }
      letter[num_byte] = static_cast<char>(*byte);
    }
    return letter;
  }

 private:
  std::set<std::string> alphabet_;
};

}  // namespace fixed_alpha_letter

#endif  // LETTER_FIXED_ALPHABET_LETTER_H_