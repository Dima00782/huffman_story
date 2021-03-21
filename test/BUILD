cc_test(
    name = "unittest",
    srcs = [
        "bit_manipulation_unittest.cc",
        "byte_istream_adapter_unittest.cc",
        "byte_ostream_adapter_unittest.cc",
        "encoding_unittest.cc",
        "huffman_tree_builder_unittest.cc",
        "text_splitter_unittest.cc",
        "trie_unittest.cc",
        "single_byte_letter_unittest.cc",
    ],
    deps = [
        "//bits_manipulation:bits_manipulation",
        "//encoding:huffman_encoding",
        "//encoding/byte_streams_adapters:byte_streams_adapters",
        "//encoding/text_splitter:text_splitter",
        "//letter:letter",
        "//letter/single_byte_letter:single_byte_letter",
        "@googletest//:gtest_main",
    ],
)
