Project Huffman
================

First class c++ project to demonstrate state of the art programming.

[![Build status](https://ci.appveyor.com/api/projects/status/oqqqcngpjjicbkoh/branch/master?svg=true)](https://ci.appveyor.com/project/Dima00782/huffman-story/branch/master)

All following useful commands are from src directory:

To build
```bash
bazel build //main:main
```

To run perftests
```bash
bazel run //perf_tests:perf_tests
```

To unittest
```bash
bazel test //test:unittest
```

To see main deps
```bash
bazel query --notool_deps --noimplicit_deps "deps(//main:main)" \--output graph > deps_graph
```
