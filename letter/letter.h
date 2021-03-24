#ifndef LETTER_LETTER_H_
#define LETTER_LETTER_H_

#include <functional>
#include <istream>
#include <memory>
#include <optional>
#include <ostream>
#include <utility>
#include <vector>

#include "bit_io/bit_writer.h"

namespace {
template <typename T>
concept Hashable = requires(T x) {
  { std::hash<T>{}(x) }
  ->std::convertible_to<std::size_t>;
};
}  // namespace

namespace letter {
template <typename T>
concept LetterConfig = Hashable<typename T::LetterType>&& requires(
    T config,
    T::LetterType letter,
    T::LetterParser letter_parser,
    std::shared_ptr<std::istream> input,
    std::ostream& output,
    bit_io::BitWriter& bit_output,
    bit_io::BitReader& bit_input) {
  { config.CreateParser(input) }
  ->std::same_as<std::unique_ptr<typename T::LetterParser>>;

  { letter_parser.Parse() }
  ->std::same_as<std::optional<typename T::LetterType>>;

  { config.Write(output, letter) }
  ->std::same_as<bool>;

  { config.WriteSerialized(bit_output, letter) }
  ->std::same_as<bool>;

  { config.ReadSerialized(bit_input) }
  ->std::same_as<std::optional<typename T::LetterType>>;
};

}  // namespace letter

#endif  // LETTER_LETTER_H_