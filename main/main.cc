#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <streambuf>
#include <string>

#include "CLI11/CLI11.hpp"
#include "encryption/huffman_encryption.h"
#include "letter/one_byte_letter.h"

int main(int argc, char* argv[]) {
  CLI::App app{"Huffman archiver"};
  app.require_subcommand(1);

  // TODO: create InstallCryptSubcommand and InstallDecryptSubcommand and use
  // them here.
  // Hmm, the library is strange, I need to pass pass a reference to
  // file_to_crypt.
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
      auto input =
          std::make_shared<std::ifstream>(file_to_crypt_path, std::ios::binary);
      auto output = std::make_shared<std::ofstream>(compressed_file_name,
                                                    std::ios::binary);
      auto letter_lexer = std::make_unique<letter::ByteLetterLexer>();
      auto letter_serializer = std::make_unique<letter::ByteLetterSerializer>();
      encryption::HuffmanEncrypt<std::byte, letter::ByteLetterLexer,
                                 letter::ByteLetterSerializer>(
          std::move(input), std::move(output), std::move(letter_lexer),
          std::move(letter_serializer));
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

      auto input = std::make_shared<std::ifstream>(file_to_decrypt_path,
                                                   std::ios::binary);
      std::filesystem::path compressed_file_name =
          file_to_decrypt_path.filename().replace_extension("");
      auto output = std::make_shared<std::ofstream>(compressed_file_name,
                                                    std::ios::binary);
      auto letter_serializer = std::make_unique<letter::ByteLetterSerializer>();
      encryption::HuffmanDecrypt<std::byte, letter::ByteLetterLexer,
                                 letter::ByteLetterSerializer>(
          std::move(input), std::move(output), std::move(letter_serializer));
    }
  });

  CLI11_PARSE(app, argc, argv);
  return 0;
}
