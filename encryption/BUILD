cc_library(
    name = "huffman_encryption",
    srcs = [
        "bit_reader.cc",
        "bit_writer.cc",
        "huffman_encryption.cc",
    ],
    hdrs = [
        "bit_reader.h",
        "bit_writer.h",
        "huffman_encryption.h",
    ],
    deps = [
        "//encryption/huffman_tree:huffman_tree",
        "//encryption/text_splitter:text_splitter",
    ],
    visibility = ["//visibility:public"],
)
