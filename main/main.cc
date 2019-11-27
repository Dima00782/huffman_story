#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <streambuf>
#include <string>

#include "encryption/huffman_encryption.h"
#include "string_io/string_bit_reader.h"
#include "string_io/string_bit_writer.h"

std::string EncryptText(const std::string& text) {
  auto string_input = std::make_unique<string_io::StringBitReader>(text);
  auto string_output = std::make_unique<string_io::StringBitWriter>();

  auto* string_output_ptr = string_output.get();
  encryption::HuffmanEncryption huffman;
  huffman.Encrypt(std::move(string_input), std::move(string_output));
  return string_output_ptr->GetData();
}

std::string DecryptText(const std::string& text) {
  auto string_input = std::make_unique<string_io::StringBitReader>(text);
  auto string_output = std::make_unique<string_io::StringBitWriter>();

  auto* string_output_ptr = string_output.get();
  encryption::HuffmanEncryption huffman;
  huffman.Decrypt(std::move(string_input), std::move(string_output));
  return string_output_ptr->GetData();
}

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
    std::cout << "passed file is not a regular file: " << file_path << std::endl;
    return 0;
  }

  if (action_flag == 'c') {
    std::ifstream input{file_path, std::ios::binary};
    const std::string content{(std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>()};
    const auto encrypted_data = EncryptText(content);
    std::filesystem::path compressed_file_name = file_path.filename();
    compressed_file_name += ".huf";
    std::ofstream output{compressed_file_name, std::ios::binary};
    output.write(encrypted_data.c_str(), encrypted_data.size());
  } else {
    assert(action_flag == 'd');
    if (!file_path.has_extension() || file_path.extension() != ".huf") {
      std::cout << "wrong extension of file: " << file_path << std::endl;
      return 0;
    }

    std::ifstream input{file_path, std::ios::binary};
    const std::string content{(std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>()};
    const auto encrypted_data = DecryptText(content);
    std::filesystem::path compressed_file_name = file_path.filename().replace_extension("");
    std::ofstream output{compressed_file_name, std::ios::binary};
    output.write(encrypted_data.c_str(), encrypted_data.size());
  }

  return 0;
}