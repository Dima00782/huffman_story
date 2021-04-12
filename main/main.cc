#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

#include "CLI11/CLI11.hpp"
#include "encoding/huffman_encoding.h"
#include "letter/fixed_alphabet_letter/fixed_alphabet_letter.h"
#include "letter/single_byte_letter/single_byte_letter.h"

namespace {

constexpr char const* kExtenstion = ".huf";
constexpr std::size_t kNumberOfRequiredSubcommands = 1u;

std::set<std::string> ReadFileTosSet(const std::filesystem::path& path) {
  std::set<std::string> set;
  std::ifstream istream{path};
  std::string str;
  while (istream >> str) {
    set.insert(str);
  }

  return set;
}

template <letter::LetterConfig Config>
void Compress(std::shared_ptr<Config> config,
              const std::filesystem::path& input_path,
              const std::filesystem::path& output_path) {
  auto input = std::make_shared<std::ifstream>(input_path, std::ios::binary);
  auto output = std::make_shared<std::ofstream>(output_path, std::ios::binary);
  encoding::HuffmanEncoder<Config>(config, std::move(input), std::move(output));
}

void InstallCompressSubcommand(CLI::App* app) {
  auto compress_command =
      app->add_subcommand("compress", "Compress passed file.");
  auto file_to_compress = std::make_shared<std::string>();
  auto output_file = std::make_shared<std::string>();
  auto alphabet_file = std::make_shared<std::string>();
  compress_command->add_option("file", *file_to_compress, "File to compress.");
  compress_command->add_option("-o,--output", *output_file, "Output file.");
  compress_command->add_option("-a,--alphabet", *alphabet_file,
                               "File with alphabet.");
  compress_command->callback([file_to_compress, output_file, alphabet_file]() {
    const std::filesystem::path file_to_compress_path{*file_to_compress};
    if (!std::filesystem::is_regular_file(file_to_compress_path)) {
      std::cerr << "It isn't a regular file : " << file_to_compress_path
                << std::endl;
      return;
    }

    std::filesystem::path compressed_file_path{*output_file};
    if (compressed_file_path.empty()) {
      compressed_file_path = std::filesystem::path{
          file_to_compress_path.filename().string() + kExtenstion};
    }

    if (alphabet_file->empty()) {
      Compress(std::make_shared<single_byte_letter::SingleByteLetterConfig>(),
               file_to_compress_path, compressed_file_path);
      return;
    }

    const std::filesystem::path alphabet_file_path{*alphabet_file};
    if (!std::filesystem::is_regular_file(alphabet_file_path)) {
      std::cerr << "It isn't a regular file : " << alphabet_file_path
                << std::endl;
      return;
    }

    auto alphabet = ReadFileTosSet(alphabet_file_path);
    Compress(std::make_shared<fixed_alpha_letter::FixedAlphabetLetterConfig>(
                 std::move(alphabet)),
             file_to_compress_path, compressed_file_path);
  });
}

template <letter::LetterConfig Config>
void Decompress(std::shared_ptr<Config> config,
                const std::filesystem::path& input_path,
                const std::filesystem::path& output_path) {
  auto input = std::make_shared<std::ifstream>(input_path, std::ios::binary);
  auto output = std::make_shared<std::ofstream>(output_path, std::ios::binary);
  encoding::HuffmanDecoder<Config>(std::move(config), std::move(input),
                                   std::move(output));
}

void InstallDecompressSubcommand(CLI::App* app) {
  auto decompress_command =
      app->add_subcommand("decompress", "Decompress passed file.");
  auto file_to_decompress = std::make_shared<std::string>();
  auto output_file = std::make_shared<std::string>();
  auto is_alphabet = std::make_shared<bool>();
  decompress_command->add_option("file", *file_to_decompress,
                                 "File to decompress.");
  decompress_command->add_option("-o,--output", *output_file, "Output file.");
  decompress_command->add_flag(
      "-a,--alphabet", *is_alphabet,
      "Decompress the file that was previously compressed with alphabet.");
  decompress_command->callback([file_to_decompress, output_file,
                                is_alphabet]() {
    const std::filesystem::path file_to_decompress_path{*file_to_decompress};
    if (!std::filesystem::is_regular_file(file_to_decompress_path)) {
      std::cerr << "It isn't a regular file : " << file_to_decompress_path
                << std::endl;
      return;
    }

    const std::filesystem::path uncompressed_file_name{*output_file};
    if (!*is_alphabet) {
      Decompress(std::make_shared<single_byte_letter::SingleByteLetterConfig>(),
                 file_to_decompress_path, uncompressed_file_name);
      return;
    }

    Decompress(std::make_shared<fixed_alpha_letter::FixedAlphabetLetterConfig>(
                   std::set<std::string>()),
               file_to_decompress_path, uncompressed_file_name);
  });
}
}  // namespace

int main(int argc, char* argv[]) {
  CLI::App app{"Huffman archiver"};
  app.require_subcommand(kNumberOfRequiredSubcommands);
  InstallCompressSubcommand(&app);
  InstallDecompressSubcommand(&app);

  CLI11_PARSE(app, argc, argv);
  return 0;
}
