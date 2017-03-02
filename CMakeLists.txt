cmake_minimum_required(VERSION 3.7)

project(RustDemangle)
enable_language(C CXX)

add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-std=c++11>)

set(RUSTDEMANGLE_SOURCE_DIR   ${CMAKE_CURRENT_SOURCE_DIR}/lib)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/googletest")

# Google Test
find_library(RUSTDEMANGLE_DEPENDENCY_GOOGLETEST
  gtest
  HINTS "${CMAKE_CURRENT_SOURCE_DIR}/googletest"
)

find_library(RUSTDEMANGLE_DEPENDENCY_GOOGLETESTMAIN
  gtest_main
  HINTS "${CMAKE_CURRENT_SOURCE_DIR}/googletest"
)

add_subdirectory(lib)
add_subdirectory(tests)
