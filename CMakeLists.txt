cmake_minimum_required(VERSION 3.4)
project(InterSignAnalysis)

set(CMAKE_C_COMPILER clang-10)
set(CMAKE_CXX_COMPILER clang++-10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# we need LLVM_HOME in order not automatically set LLVM_DIR
if(NOT DEFINED ENV{LLVM_HOME})
    message(FATAL_ERROR "$LLVM_HOME is not defined")
else ()
    set(ENV{LLVM_DIR} $ENV{LLVM_HOME}/lib/cmake/llvm)
endif()

find_package(LLVM REQUIRED CONFIG)
add_definitions(${LLVM_DEFINITIONS})

include_directories($ENV{LLVM_HOME}/include)
link_directories(${LLVM_LIBRARY_DIRS})


add_subdirectory(hw7)