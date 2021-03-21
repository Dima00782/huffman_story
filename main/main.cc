#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

#include "CLI11/CLI11.hpp"
#include "encoding/huffman_encoding.h"
#include "letter/single_byte_letter/single_byte_letter.h"

namespace {

constexpr char const* kExtenstion = ".huf";

template <letter::LetterConfig Config>
void Compress(std::shared_ptr<Config> config,
              const std::filesystem::path& input_path,
              const std::filesystem::path& output_path) {
  auto input = std::make_shared<std::ifstream>(input_path, std::ios::binary);
  auto output = std::make_shared<std::ofstream>(output_path, std::ios::binary);
  encoding::HuffmanEncoder<Config>(config, std::move(input), std::move(output));
}

template <letter::LetterConfig Config>
void InstallCompressSubcommand(CLI::App* app, std::shared_ptr<Config> config) {
  auto compress_command =
      app->add_subcommand("compress", "Compress passed file.");
  auto file_to_compress = std::make_shared<std::string>();
  compress_command->add_option("file", *file_to_compress, "File to compress.");
  compress_command->callback([file_to_compress, config]() {
    const std::filesystem::path file_to_compress_path{*file_to_compress};
    if (!std::filesystem::is_regular_file(file_to_compress_path)) {
      std::cerr << "It isn't a regular file : " << file_to_compress_path
                << std::endl;
    } else {
      std::filesystem::path compressed_file_path{
          file_to_compress_path.filename().string() + kExtenstion};
      Compress(std::move(config), file_to_compress_path, compressed_file_path);
    }
  });
}

template <letter::LetterConfig Config>
void Decompress(std::shared_ptr<Config> config,
                const std::filesystem::path& input_path,
                const std::filesystem::path& output_path) {
  auto input = std::make_shared<std::ifstream>(input_path, std::ios::binary);
  auto output = std::make_shared<std::ofstream>(output_path, std::ios::binary);
  encoding::HuffmanDecoder<letter::SingleByteLetterConfig>(
      std::move(config), std::move(input), std::move(output));
}

template <letter::LetterConfig Config>
void InstallDecompressSubcommand(CLI::App* app,
                                 std::shared_ptr<Config> config) {
  auto decompress_command = app->add_subcommand(
      "decompress", "Decompress passed file. File must have .huf extension.");
  auto file_to_decompress = std::make_shared<std::string>();
  decompress_command->add_option("file", *file_to_decompress,
                                 "File to decompress.");
  decompress_command->callback([file_to_decompress, config]() {
    const std::filesystem::path file_to_decompress_path{*file_to_decompress};
    if (!std::filesystem::is_regular_file(file_to_decompress_path)) {
      std::cerr << "It isn't a regular file : " << file_to_decompress_path
                << std::endl;
    } else {
      if (!file_to_decompress_path.has_extension() ||
          file_to_decompress_path.extension() != kExtenstion) {
        std::cerr << "wrong extension of file: " << file_to_decompress_path
                  << std::endl;
        return;
      }

      std::filesystem::path uncompressed_file_name =
          file_to_decompress_path.filename().replace_extension("");
      Decompress(std::move(config), file_to_decompress_path,
                 uncompressed_file_name);
    }
  });
}
}  // namespace

int main(int argc, char* argv[]) {
  auto config = std::make_shared<letter::SingleByteLetterConfig>();
  CLI::App app{"Huffman archiver"};
  app.require_subcommand(1);
  InstallCompressSubcommand(&app, config);
  InstallDecompressSubcommand(&app, config);

  CLI11_PARSE(app, argc, argv);
  return 0;
}
