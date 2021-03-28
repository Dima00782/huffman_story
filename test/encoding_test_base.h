#ifndef TEST_ENCODING_TEST_BASE_H_
#define TEST_ENCODING_TEST_BASE_H_

#include "gtest/gtest.h"

#include <memory>
#include <sstream>
#include <string>

#include "encoding/huffman_encoding.h"
#include "letter/letter.h"

namespace test {

template <letter::LetterConfig Config>
std::string EncryptTextBase(const std::string& text,
                            std::shared_ptr<Config> config) {
  auto input = std::make_shared<std::istringstream>(text);
  auto output = std::make_shared<std::ostringstream>();
  encoding::HuffmanEncoder<Config>(std::move(config), input, output);
  return output->str();
}

template <letter::LetterConfig Config>
std::string DecryptTextBase(const std::string& text,
                            std::shared_ptr<Config> config) {
  auto input = std::make_shared<std::istringstream>(text);
  auto output = std::make_shared<std::ostringstream>();
  encoding::HuffmanDecoder<Config>(std::move(config), input, output);
  return output->str();
}

}  // namespace test

#endif  // TEST_ENCODING_TEST_BASE_H_
