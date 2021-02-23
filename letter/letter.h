#include <functional>
#include <istream>
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
concept LetterConfig =
    Hashable<typename T::LetterType>&& requires(T config,
                                                typename T::LetterType letter,
                                                std::istream& input,
                                                std::ostream& output,
                                                bit_io::BitWriter& bit_output,
                                                bit_io::BitReader& bit_input) {
  { config.Parse(input) }
  ->std::same_as<std::vector<typename T::LetterType>>;

  { config.Write(output, letter) }
  ->std::same_as<bool>;

  { config.WriteSerialized(bit_output, letter) }
  ->std::same_as<bool>;

  { config.ReadSerialized(bit_input) }
  ->std::same_as<std::optional<typename T::LetterType>>;
};

}  // namespace letter