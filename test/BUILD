cc_test(
    name = "unittest",
    srcs = [
        "char_ostream_adapter_unittest.cc",
        "encryption_unittest.cc",
        "huffman_tree_builder_unittest.cc",
        "text_splitter_unittest.cc",
        "trie_unittest.cc",
    ],
    deps = [
        "//encryption:huffman_encryption",
        "//encryption/char_streams_adapters:char_streams_adapters",
        "//encryption/text_splitter:text_splitter",
        "@googletest//:gtest_main",
    ],
)
