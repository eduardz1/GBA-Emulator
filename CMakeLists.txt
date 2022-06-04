cmake_minimum_required(VERSION 3.22)
if(CMAKE_HOST_WIN32)
	set(CMAKE_TOOLCHAIN_FILE C:/Users/occhi/Github/GBA-emulator/lib/vcpkg/scripts/buildsystems/vcpkg.cmake)
endif(CMAKE_HOST_WIN32)
project(GBA_emulator)

set(CMAKE_CXX_STANDARD 14)

option(CMAKE_USE_WIN32_THREADS_INIT "using WIN32 threads" ON)
option(gtest_disable_pthreads "Disable uses of pthreads in gtest." ON)

add_subdirectory(src)
add_subdirectory(lib/googletest)

#-DCMAKE_TOOLCHAIN_FILE=C:/Users/occhi/Github/GBA-emulator/lib/vcpkg/scripts/buildsystems/vcpkg.cmake