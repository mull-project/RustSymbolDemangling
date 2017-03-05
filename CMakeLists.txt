cmake_minimum_required(VERSION 3.7)

project(RustSymbolDemangling)
enable_language(C CXX)

add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-std=c++11>)

set(RSD_SOURCE_DIR   ${CMAKE_CURRENT_SOURCE_DIR}/lib)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/googletest")

# Google Test
find_library(RSD_DEPENDENCY_GOOGLETEST
  gtest
  HINTS "${CMAKE_CURRENT_SOURCE_DIR}/googletest"
)

find_library(RSD_DEPENDENCY_GOOGLETESTMAIN
  gtest_main
  HINTS "${CMAKE_CURRENT_SOURCE_DIR}/googletest"
)

add_subdirectory(lib)
add_subdirectory(tests)
