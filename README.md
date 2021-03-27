Project Huffman
================

Handmade c++ project to demonstrate state-of-the-art programming.

[![Build status](https://ci.appveyor.com/api/projects/status/nht8ih7mml2s6sk9/branch/master?svg=true)](https://ci.appveyor.com/project/dbezhetskov49849/huffman-story/branch/master)

All following useful commands are from src directory:

To build
```bash
bazel build //main:main
```

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
