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
    typename T::LetterType letter,
    typename T::LetterLexerType lexer,
    std::shared_ptr<std::istream> input,
    typename T::LetterSerializerType serializer,
    bit_io::BitWriter& bit_output,
    bit_io::BitReader& bit_input) {
  lexer.Split(input);
  { serializer.WriteSerialized(bit_output, letter) }
  ->std::same_as<bool>;
  { serializer.ReadSerialized(bit_input) }
  ->std::same_as<std::optional<typename T::LetterType>>;
};

}  // namespace letter