# Make sure to delete CMake's CMakeCache.txt file or even the whole build folder.
# Before bootstrapping this file as a CMake toolchain.
# "CMake ignores toolchain file. How do get debug/error output?"
# http://stackoverflow.com/a/8303317/598057

MESSAGE(STATUS "Using RustDemangle.toolchain.OSX.cmake")

# brew install llvm
set(LLVM_ROOT "/usr/local/opt/llvm" CACHE PATH "Root of LLVM install.")

# A bit of a sanity check:
if (NOT EXISTS ${LLVM_ROOT}/include/llvm)
message(SEND_ERROR "LLVM_ROOT (${LLVM_ROOT}) is not a valid LLVM install")
endif()

## Changing the compilers will not work in Xcode which always forces Apple Clang,
## It will always yield something like this:
## -- The C compiler identification is AppleClang 8.0.0.8000042
## -- The CXX compiler identification is AppleClang 8.0.0.8000042
## Without looking at the variables below. Only Ninja will work.
# set(CMAKE_C_COMPILER   ${LLVM_ROOT}/bin/clang)
# set(CMAKE_CXX_COMPILER ${LLVM_ROOT}/bin/clang++)
# set(CMAKE_ASM_COMPILER ${LLVM_ROOT}/bin/llvm-as)

# set(CMAKE_MACOSX_RPATH ON)
# SET(CMAKE_BUILD_WITH_INSTALL_RPATH ON)
