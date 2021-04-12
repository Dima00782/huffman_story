Project Huffman
================

Handmade c++ project to demonstrate state-of-the-art programming.

[![Build status](https://ci.appveyor.com/api/projects/status/nht8ih7mml2s6sk9/branch/master?svg=true)](https://ci.appveyor.com/project/dbezhetskov49849/huffman-story/branch/master)

## Usage

### basic mode
```bash
huffman compress /path/to/your/file
huffman decompress /path/to/your/file.huf
```
The archiver will use ascii alphabet to count frequencies, for example: `"aabbde"` -> `{a: 2, b: 2, d: 1, e: 1}`

### alphabet mode:
```bash
huffman compress /path/to/your/file --alphabet /path/to/your/alphabet_file 
huffman decompress /path/to/your/file.huf --alphabet
```
Here you can specify your own letters as words in alphabet_file
and the archiver will use the union of your alphabet and the ascii alphabet to count frequencies.

for example:
alpabet file = `{"aa", "de"}`, text = `"aabbde"` -> `{aa: 1, b: 2, de: 1}`.

see some more useful example in the /perf_tests/benchmark.py:alphabet_letter_benchmark 

## Building from sources
```bash
bazel build //main:main --config=release
```

## Other useful commands:

To run perftests
```bash
bazel run //perf_tests:benchmark --config=release
```

To unittest
```bash
bazel test //test:unittest [--config={asan, ubsan}]
```

To see main deps
```bash
bazel query --notool_deps --noimplicit_deps "deps(//main:main)" \--output graph > deps_graph
```
