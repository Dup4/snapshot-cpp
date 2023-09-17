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

## Usage

[![asciicast](https://asciinema.org/a/608449.svg)](https://asciinema.org/a/608449)

```cpp
const std::string tmp = "snapshot-cpp";
const std::string tmp_diff = tmp + "-diff";

EXPECT_EQ(tmp, SNAPSHOT_INLINE(tmp));
// EXPECT_EQ(tmp, std::string("snapshot-cpp"));

SNAPSHOT(tmp);
// // /home/Dup4/snapshot-cpp/test/snapshot_test.cc.TestBody.58.0
// std::string("snapshot-cpp")

SNAPSHOT_DIFF(tmp, tmp_diff);
// // /home/Dup4/snapshot-cpp/test/snapshot_test.cc.TestBody.59.0
// // before
// std::string("snapshot-cpp")
// // after
// std::string("snapshot-cpp-diff")
// // diff
// diff --git a/tmp/snapshot_test.cc.TestBody.59.0.snap.before b/tmp/snapshot_test.cc.TestBody.59.0.snap.after
// index aeb786e..3982900 100644
// --- a/tmp/snapshot_test.cc.TestBody.59.0.snap.before
// +++ b/tmp/snapshot_test.cc.TestBody.59.0.snap.after
// @@ -1 +1 @@
// -std::string("snapshot-cpp")
// \ No newline at end of file
// +std::string("snapshot-cpp-diff")
// \ No newline at end of file
```

[clang-format-badge]: https://github.com/Dup4/snapshot-cpp/workflows/Clang%20Format/badge.svg
[clang-format-action]: https://github.com/Dup4/snapshot-cpp/actions/workflows/clang_format.yml
[test-badge]: https://github.com/Dup4/snapshot-cpp/workflows/Test/badge.svg
[test-action]: https://github.com/Dup4/snapshot-cpp/actions/workflows/test.yml
[codecov-badge]: https://codecov.io/gh/Dup4/snapshot-cpp/branch/main/graph/badge.svg
[codecov]: https://codecov.io/gh/Dup4/snapshot-cpp
[release-badge]: https://img.shields.io/github/release/Dup4/snapshot-cpp.svg
[release-page]: https://GitHub.com/Dup4/snapshot-cpp/releases/
[jest-snapshot]: https://jestjs.io/docs/snapshot-testing
