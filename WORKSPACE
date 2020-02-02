load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_file")

git_repository(
    name = "googletest",
    remote = "https://github.com/google/googletest",
    tag = "release-1.8.1",
)

http_file(
    name = "CLI11",
    downloaded_file_path = "CLI11.hpp",
    urls = ["https://github.com/CLIUtils/CLI11/releases/download/v1.9.0/CLI11.hpp"],
    sha256 = "6f0a1d8846ed7fa4c2b66da3eb252aa03d27170258dfeb093b258e0afebb8c83",
)
