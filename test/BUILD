cc_test(
    name = "unittest",
    srcs = [
        "byte_aligned_bit_reader_unittest.cc",
        "byte_aligned_bit_writer_unittest.cc",
        "encryption_unittest.cc",
        "huffman_tree_builder_unittest.cc",
        "string_io_unittest.cc",
    ],
    deps = [
        "//encryption:huffman_encryption",
        "//string_io:string_io",
        "@googletest//:gtest_main",
    ],
)
