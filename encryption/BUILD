cc_library(
    name = "huffman_encryption",
    srcs = [
        "bit_reader.cc",
        "bit_writer.cc",
        "byte_aligned_bit_reader.cc",
        "huffman_encryption.cc",
    ],
    hdrs = [
        "bit_reader.h",
        "bit_writer.h",
        "byte_aligned_bit_reader.h",
        "huffman_encryption.h",
    ],
    deps = [
        "//bits_manipulation:bits_manipulation",
        "//encryption/huffman_tree:huffman_tree",
    ],
    visibility = ["//visibility:public"],
)
