cc_test(
    name = "unittest",
    srcs = [
        "char_istream_adapter_unittest.cc",
        "char_ostream_adapter_unittest.cc",
        "encoding_unittest.cc",
        "huffman_tree_builder_unittest.cc",
        "text_splitter_unittest.cc",
        "trie_unittest.cc",
        "one_byte_letter_unittest.cc",
    ],
    deps = [
        "//encoding:huffman_encoding",
        "//encoding/char_streams_adapters:char_streams_adapters",
        "//encoding/text_splitter:text_splitter",
        "//letter:letter",
        "//letter:one_byte_letter",
        "@googletest//:gtest_main",
    ],
)
