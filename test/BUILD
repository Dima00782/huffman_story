cc_library(
    name = "string_bit_io",
    hdrs = [
        "string_bit_io.h",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "encoding_base",
    hdrs = [
        "encoding_test_base.h",
    ],
    visibility = ["//visibility:public"],
)

cc_test(
    name = "unittest",
    srcs = [
        "bit_manipulation_unittest.cc",
        "byte_istream_adapter_unittest.cc",
        "byte_ostream_adapter_unittest.cc",
        "encoding_single_byte_letter_unittest.cc",
        "encoding_fixed_alphabet_letter_unittest.cc",
        "huffman_tree_builder_unittest.cc",
        "trie_unittest.cc",
        "single_byte_letter_unittest.cc",
        "fixed_alphabet_letter_unittest.cc",
    ],
    deps = [
        "//test:string_bit_io",
        "//test:encoding_base",
        "//bits_manipulation:bits_manipulation",
        "//encoding:huffman_encoding",
        "//encoding/byte_streams_adapters:byte_streams_adapters",
        "//letter:letter",
        "//letter/single_byte_letter:single_byte_letter",
        "//letter/fixed_alphabet_letter:fixed_alphabet_letter",
        "//letter/fixed_alphabet_letter:trie",
        "@googletest//:gtest_main",
    ],
)
