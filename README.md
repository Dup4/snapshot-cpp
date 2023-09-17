<img align="right" width="96px" src="./assets/1200px_cpp_logo.svg.png">

# snapshot-cpp

[![Clang Format][clang-format-badge]][clang-format-action]
[![Test][test-badge]][test-action]
[![codecov][codecov-badge]][codecov]
[![GitHub release][release-badge]][release-page]

Similar to [Jest Snapshot][jest-snapshot], but implemented in C++.

## Feature

* `SNAPSHOT(content)`
* `SNAPSHOT_INLINE(content)`
* `SNAPSHOT_DIFF(before_content, after_content)` required `git` binary.
* Support Linux and macOS.

[clang-format-badge]: https://github.com/Dup4/snapshot-cpp/workflows/Clang%20Format/badge.svg
[clang-format-action]: https://github.com/Dup4/snapshot-cpp/actions/workflows/clang_format.yml
[test-badge]: https://github.com/Dup4/snapshot-cpp/workflows/Test/badge.svg
[test-action]: https://github.com/Dup4/snapshot-cpp/actions/workflows/test.yml
[codecov-badge]: https://codecov.io/gh/Dup4/snapshot-cpp/branch/main/graph/badge.svg
[codecov]: https://codecov.io/gh/Dup4/snapshot-cpp
[release-badge]: https://img.shields.io/github/release/Dup4/snapshot-cpp.svg
[release-page]: https://GitHub.com/Dup4/snapshot-cpp/releases/
[jest-snapshot]: https://jestjs.io/docs/snapshot-testing
