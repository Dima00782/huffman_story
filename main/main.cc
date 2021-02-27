#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

#include "CLI11/CLI11.hpp"
#include "encoding/huffman_encoding.h"
#include "letter/one_byte_letter.h"

namespace {

constexpr char const* kExtenstion = ".huf";

template <letter::LetterConfig Config>
void Crypt(std::shared_ptr<Config> config,
           const std::filesystem::path& input_path,
           const std::filesystem::path& output_path) {
  auto input = std::make_shared<std::ifstream>(input_path, std::ios::binary);
  auto output = std::make_shared<std::ofstream>(output_path, std::ios::binary);
  encoding::HuffmanEncrypt<Config>(config, std::move(input), std::move(output));
}

template <letter::LetterConfig Config>
void InstallCryptSubcommand(CLI::App* app, std::shared_ptr<Config> config) {
  auto crypt_command = app->add_subcommand("crypt", "Crypt passed file.");
  auto file_to_crypt = std::make_shared<std::string>();
  crypt_command->add_option("file", *file_to_crypt, "File to crypt.");
  crypt_command->callback([file_to_crypt, config]() {
    const std::filesystem::path file_to_crypt_path{*file_to_crypt};
    if (!std::filesystem::is_regular_file(file_to_crypt_path)) {
      std::cerr << "It isn't a regular file : " << file_to_crypt_path
                << std::endl;
    } else {
      std::filesystem::path compressed_file_path{
          file_to_crypt_path.filename().string() + kExtenstion};
      Crypt(std::move(config), file_to_crypt_path, compressed_file_path);
    }
  });
}

template <letter::LetterConfig Config>
void Decrypt(std::shared_ptr<Config> config,
             const std::filesystem::path& input_path,
             const std::filesystem::path& output_path) {
  auto input = std::make_shared<std::ifstream>(input_path, std::ios::binary);
  auto output = std::make_shared<std::ofstream>(output_path, std::ios::binary);
  encoding::HuffmanDecrypt<letter::OneByteLetterConfig>(
      std::move(config), std::move(input), std::move(output));
}

template <letter::LetterConfig Config>
void InstallDecryptSubcommand(CLI::App* app, std::shared_ptr<Config> config) {
  auto decrypt_command = app->add_subcommand(
      "decrypt", "Decrypt passed file. File must have .huf extension.");
  auto file_to_decrypt = std::make_shared<std::string>();
  decrypt_command->add_option("file", *file_to_decrypt, "File to decrypt.");
  decrypt_command->callback([file_to_decrypt, config]() {
    const std::filesystem::path file_to_decrypt_path{*file_to_decrypt};
    if (!std::filesystem::is_regular_file(file_to_decrypt_path)) {
      std::cerr << "It isn't a regular file : " << file_to_decrypt_path
                << std::endl;
    } else {
      if (!file_to_decrypt_path.has_extension() ||
          file_to_decrypt_path.extension() != kExtenstion) {
        std::cerr << "wrong extension of file: " << file_to_decrypt_path
                  << std::endl;
        return;
      }

      std::filesystem::path uncompressed_file_name =
          file_to_decrypt_path.filename().replace_extension("");
      Decrypt(std::move(config), file_to_decrypt_path, uncompressed_file_name);
    }
  });
}
}  // namespace

int main(int argc, char* argv[]) {
  auto config = std::make_shared<letter::OneByteLetterConfig>();

  CLI::App app{"Huffman archiver"};
  app.require_subcommand(1);
  InstallCryptSubcommand(&app, config);
  InstallDecryptSubcommand(&app, config);

  CLI11_PARSE(app, argc, argv);
  return 0;
}
