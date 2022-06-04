cmake_minimum_required(VERSION 3.22)
project(GBA_emulator)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(src)
add_subdirectory(src/cpu/test)
add_subdirectory(lib/googletest)