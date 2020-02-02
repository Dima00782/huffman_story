#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <streambuf>
#include <string>

#include "CLI11/CLI11.hpp"
#include "char_streams_adapters/char_istream_adapter.h"
#include "char_streams_adapters/char_ostream_adapter.h"
#include "encryption/huffman_encryption.h"
#include "string_io/string_bit_reader.h"
#include "string_io/string_bit_writer.h"

std::set<std::string> GetAllCharactersAlphabet();
std::set<std::string> GetAlphabetFromFile(const std::string& file_name);
std::set<std::string> UnionTwoAlphabets(const std::set<std::string>& lhs,
                                        const std::set<std::string>& rhs);

int main(int argc, char* argv[]) {
  CLI::App app{"Huffman archiver"};
  app.require_subcommand(1);

  auto crypt_command = app.add_subcommand("crypt", "Crypt passed file.");
  std::string file_to_crypt;
  crypt_command->add_option("file", file_to_crypt, "File to crypt.");
  std::string alphabet_file;
  crypt_command->add_option("-a,--alphabet", alphabet_file,
                            "File with alphabet.");
  crypt_command->callback([&file_to_crypt, &alphabet_file]() {
    const std::filesystem::path file_to_crypt_path{file_to_crypt};
    if (!std::filesystem::is_regular_file(file_to_crypt_path)) {
      std::cerr << "It isn't a regular file : " << file_to_crypt_path
                << std::endl;
    } else {
      std::filesystem::path compressed_file_name =
          file_to_crypt_path.filename();
      compressed_file_name += ".huf";
      auto output = std::make_shared<char_adapters::CharOStreamAdapter>(
          std::make_shared<std::ofstream>(compressed_file_name,
                                          std::ios::binary));
      auto alphabet = GetAllCharactersAlphabet();
      if (!alphabet_file.empty()) {
        alphabet =
            UnionTwoAlphabets(GetAlphabetFromFile(alphabet_file), alphabet);
      }
      encryption::HuffmanEncrypt(
          std::make_shared<std::ifstream>(file_to_crypt_path, std::ios::binary),
          std::move(output), alphabet);
    }
  });

  auto decrypt_command = app.add_subcommand(
      "decrypt", "Decrypt passed file. File must have .huf extension.");
  std::string file_to_decrypt;
  decrypt_command->add_option("file", file_to_decrypt, "File to decrypt.");
  decrypt_command->callback([&file_to_decrypt]() {
    const std::filesystem::path file_to_decrypt_path{file_to_decrypt};
    if (!std::filesystem::is_regular_file(file_to_decrypt_path)) {
      std::cerr << "It isn't a regular file : " << file_to_decrypt_path
                << std::endl;
    } else {
      if (!file_to_decrypt_path.has_extension() ||
          file_to_decrypt_path.extension() != ".huf") {
        std::cerr << "wrong extension of file: " << file_to_decrypt_path
                  << std::endl;
        return;
      }

      auto input = std::make_shared<char_adapters::CharIStreamAdapter>(
          std::make_shared<std::ifstream>(file_to_decrypt_path,
                                          std::ios::binary));
      std::filesystem::path compressed_file_name =
          file_to_decrypt_path.filename().replace_extension("");
      encryption::HuffmanDecrypt(std::move(input),
                                 std::make_shared<std::ofstream>(
                                     compressed_file_name, std::ios::binary));
    }
  });

  CLI11_PARSE(app, argc, argv);
  return 0;
}

std::set<std::string> GetAllCharactersAlphabet() {
  std::set<std::string> alphabet;
  for (int letter = 0; letter < 256; ++letter) {
    alphabet.insert(std::string{static_cast<char>(letter)});
  }
  return alphabet;
}

std::set<std::string> GetAlphabetFromFile(const std::string& file_name) {
  std::ifstream alphabet_file(file_name);

  std::set<std::string> alphabet;
  std::string letter;
  while (alphabet_file >> letter) {
    alphabet.insert(letter);
  }

  return alphabet;
}

std::set<std::string> UnionTwoAlphabets(const std::set<std::string>& lhs,
                                        const std::set<std::string>& rhs) {
  auto alphabet = lhs;
  alphabet.insert(rhs.cbegin(), rhs.cend());
  return alphabet;
}
