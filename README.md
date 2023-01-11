<img align="right" width="96px" src="./assets/1200px_cpp_logo.svg.png">

# snapshot-cpp

[![Clang Format](https://github.com/Dup4/snapshot-cpp/workflows/Clang%20Format/badge.svg)](https://github.com/Dup4/snapshot-cpp/actions/workflows/clang_format.yml)
[![Test](https://github.com/Dup4/snapshot-cpp/workflows/Test/badge.svg)](https://github.com/Dup4/snapshot-cpp/actions/workflows/test.yml)
[![codecov](https://codecov.io/gh/Dup4/snapshot-cpp/branch/main/graph/badge.svg)](https://codecov.io/gh/Dup4/snapshot-cpp)
[![GitHub release](https://img.shields.io/github/release/Dup4/snapshot-cpp.svg)](https://GitHub.com/Dup4/snapshot-cpp/releases/)

Similar to [Jest Snapshot](https://jestjs.io/docs/snapshot-testing), but implemented in C++.

## Feature

* `SNAPSHOT(content)`
* `SNAPSHOT_INLINE(content)`
* `SNAPSHOT_DIFF(before_content, after_content)` required `git` binary.
* Only support Linux and macOS.
