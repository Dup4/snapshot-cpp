include(FetchContent)

FetchContent_Declare(
    gtest
    QUIET
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.11.0
)

FetchContent_Declare(
    gbenchmark
    QUIET
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG v1.6.0
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(BUILD_GMOCK ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(gtest gbenchmark)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-result")

add_executable(
    unit_test
    ${PROJECT_SOURCE_DIR}/test/snapshot_test.cc
    ${PROJECT_SOURCE_DIR}/test/internal/file_utility_test.cc
    ${PROJECT_SOURCE_DIR}/test/internal/string_utility_test.cc
    ${PROJECT_SOURCE_DIR}/test/internal/system_utility_test.cc
    ${PROJECT_SOURCE_DIR}/test/internal/utility_test.cc
)

target_link_libraries(
    unit_test
PRIVATE
    gtest_main
)

# automatic discovery of unit tests
include(GoogleTest)
gtest_discover_tests(
    unit_test
PROPERTIES
    LABELS "unit"
DISCOVERY_TIMEOUT  # how long to wait (in seconds) before crashing
    240
)

add_executable(
    unit_benchmark
    ${PROJECT_SOURCE_DIR}/test/snapshot_benchmark.cc
)

target_link_libraries(
    unit_benchmark
PRIVATE
    benchmark::benchmark
    benchmark::benchmark_main
)
