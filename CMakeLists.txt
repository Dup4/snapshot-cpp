option(SNAPSHOT_BUILD_TESTS "Build all of snapshot's own tests." OFF)

cmake_minimum_required(VERSION 3.14)

project(snapshot)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# place binaries and libraries according to GNU standards
include(GNUInstallDirs)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

include_directories(
    ${PROJECT_SOURCE_DIR}/include/
)

add_library(snapshot INTERFACE)
target_include_directories(snapshot INTERFACE ${PROJECT_SOURCE_DIR}/include/)

if (SNAPSHOT_BUILD_TESTS)

enable_testing()

add_subdirectory(test/)

endif()
