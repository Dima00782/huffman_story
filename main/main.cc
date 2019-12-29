#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <streambuf>
#include <string>

#include "char_streams_adapters/char_istream_adapter.h"
#include "char_streams_adapters/char_ostream_adapter.h"
#include "encryption/huffman_encryption.h"
#include "string_io/string_bit_reader.h"
#include "string_io/string_bit_writer.h"

int main(int argc, char* argv[]) {
  if (argc != 3) {
    std::cout << "usage:" << std::endl;
    std::cout << "huffman -c file_to_compress" << std::endl;
    std::cout << "huffman -d file_to_decompress" << std::endl;
    return -1;
  }

  std::string first_param{argv[1]};
  if (first_param.size() != 2) {
    std::cout << "unknown parameter: " << first_param << std::endl;
    return -2;
  }

  const char dash = first_param[0];
  const char action_flag = first_param[1];
  if (dash != '-' || (action_flag != 'c' && action_flag != 'd')) {
    std::cout << "unknown parameter: " << action_flag << std::endl;
    return -2;
  }

  const std::filesystem::path file_path{argv[2]};
  if (!std::filesystem::is_regular_file(file_path)) {
    std::cout << "passed file is not a regular file: " << file_path
              << std::endl;
    return 0;
  }

  if (action_flag == 'c') {
    std::filesystem::path compressed_file_name = file_path.filename();
    compressed_file_name += ".huf";
    auto output = std::make_shared<char_adapters::CharOStreamAdapter>(
        std::make_shared<std::ofstream>(compressed_file_name,
                                        std::ios::binary));
    encryption::HuffmanEncrypt(
        std::make_shared<std::ifstream>(file_path, std::ios::binary),
        std::move(output));
  } else {
    assert(action_flag == 'd');
    if (!file_path.has_extension() || file_path.extension() != ".huf") {
      std::cout << "wrong extension of file: " << file_path << std::endl;
      return 0;
    }

    auto input = std::make_shared<char_adapters::CharIStreamAdapter>(
        std::make_shared<std::ifstream>(file_path, std::ios::binary));
    std::filesystem::path compressed_file_name =
        file_path.filename().replace_extension("");
    encryption::HuffmanDecrypt(
        std::move(input), std::make_shared<std::ofstream>(compressed_file_name,
                                                          std::ios::binary));
  }

  return 0;
}
