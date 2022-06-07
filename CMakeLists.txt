cmake_minimum_required(VERSION 3.22)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

project(GBA_emulator LANGUAGES CXX)

# First a conan.cmake is downloaded, if not present. This file handles all of the "Conan"-y things with cmake... I guess.
if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
    message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
    file(DOWNLOAD  "https://raw.githubusercontent.com/conan-io/cmake-conan/master/conan.cmake"
        "${CMAKE_BINARY_DIR}/conan.cmake")
endif()
include(${CMAKE_BINARY_DIR}/conan.cmake)

################################################################################
################################################################################

option(ONLY_COVERAGE "Build only tests necessary for coverage" FALSE)
option(LIBCPP "Build with libc++" FALSE)
option(ENABLE_COVERAGE "Enable coverage reporting for gcc/clang" FALSE)
option(ENABLE_ASAN "Enable address sanitizer" FALSE)
option(BUILD_SHARED_LIBS "Enable compilation of shared libraries" FALSE)
option(ENABLE_TESTING "Enable the building of the test" TRUE)
option(ENABLE_CLANG_TIDY "Enable testing with clang-tidy" FALSE)
option(ENABLE_CPPCHECK "Enable testing with cppcheck" FALSE)
option(SIMPLE_BUILD "Build the project as minimally as possible" FALSE)
option(BUILD_DOC "Build the project's documentation" OFF)
option(FORCE_COLORED_OUTPUT "Always produce ANSI-colored output (GNU/Clang only)." TRUE)
option(DEBUG_LOGGING "Enabling debug logging" FALSE)
option(CMAKE_USE_WIN32_THREADS_INIT "using WIN32 threads" ON)
option(gtest_disable_pthreads "Disable uses of pthreads in gtest." ON)

################################################################################
################################################################################

message("+++++++${PROJECT_NAME}++++++++++")

set(CMAKE_CXX_STANDARD 20)

add_library(project_warnings INTERFACE)
add_library(project_options INTERFACE)

target_compile_features(project_options INTERFACE cxx_std_20)

if(ONLY_COVERAGE OR ENABLE_COVERAGE)
    target_compile_options(project_options INTERFACE --coverage -O0 -g)
    target_link_libraries(project_options INTERFACE --coverage)
endif()

if(ENABLE_ASAN)
    target_compile_options(project_options INTERFACE -fsanitize=address)
    target_link_libraries(project_options INTERFACE -fsanitize=address)
endif()

target_compile_options(project_warnings
    INTERFACE
    -Wall
    -Wextra
    -Wshadow
    -Wnon-virtual-dtor
    -Wold-style-cast
    -Wcast-align
    -Wunused
    -Woverloaded-virtual
    -Wpedantic
    -Wconversion
    -Wsign-conversion
    -Wnull-dereference
    -Wdouble-promotion
    -Wformat=2)

if(CMAKE_HOST_WIN32)
		set(CMAKE_TOOLCHAIN_FILE C:/Users/occhi/Github/GBA-emulator/lib/vcpkg/scripts/buildsystems/vcpkg.cmake)
endif(CMAKE_HOST_WIN32)

# some GCC specific warnings. These flags are added only if the used compiler is GCC.
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_compile_options(project_warnings
        INTERFACE
        -Wmisleading-indentation
        -Wduplicated-cond
        -Wlogical-op
        -Wuseless-cast
        )
    target_link_libraries(project_options INTERFACE stdc++fs)
endif()

if (${FORCE_COLORED_OUTPUT})
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        target_compile_options (project_options INTERFACE -fdiagnostics-color=always)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        target_compile_options (project_options INTERFACE -fcolor-diagnostics)
    endif ()
endif ()

find_program(CCACHE ccache)
if(CCACHE)
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE})
endif()

if(ENABLE_CPPCHECK)
    find_program(CPPCHECK cppcheck)
    if(CPPCHECK)
    set(CMAKE_CXX_CPPCHECK
        ${CPPCHECK}
        --suppress=syntaxError
        --enable=all
        --inconclusive)
    else()
    message(SEND_ERROR "cppcheck requested but executable not found")
    endif()
endif()

if(ENABLE_CLANG_TIDY)
    find_program(CLANGTIDY clang-tidy)
    if(CLANGTIDY)
    set(CMAKE_CXX_CLANG_TIDY ${CLANGTIDY})
    else()
    message(SEND_ERROR "clang-tidy requested but executable not found")
    endif()
endif()

if(BUILD_DOC)
    find_package(Doxygen)
    if (DOXYGEN_FOUND)
                SET(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile.in)
                SET(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

                configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

                add_custom_target(doc ALL
                        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
                        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                        COMMENT "Buidling Doxygen documentation"
                        VERBATIM )
    else (DOXYGEN_FOUND)
                message("No doxygen binary found on the system.")
                SET(${BUILD_DOC} OFF)
    endif ()
endif()

if(ENABLE_TESTING)
    add_subdirectory(lib/googletest)
    enable_testing()

    # we don't add REQUIRED because it's just for testing
    find_package(GTest)
    if(GTEST_FOUND)
        add_executable(unit_tests ${sources_test} ${sources})

        # we add this define to prevent collision with the main
        # this might be better solved by not adding the source with the main to the
        # testing target
        target_compile_definitions(unit_tests PUBLIC UNIT_TESTS)

        # this allows us to use our executable as a link library
        # therefore we can inherit all compiler options and library dependencies
        set_target_properties(example PROPERTIES ENABLE_EXPORTS on)

        target_link_libraries(unit_tests PUBLIC
          ${GTEST_BOTH_LIBRARIES}
          example
        )

        target_include_directories(unit_tests PUBLIC
          ${GTEST_INCLUDE_DIRS} # doesn't do anything on Linux
        )
    else()
        message(SEND_ERROR "could not find gtest library")
    endif()
endif()

add_subdirectory(src)

message("#########################################")
message("\t \t Summary")
message("#########################################")
message("Build type:       \t ${CMAKE_BUILD_TYPE}")
message("Install prefix:   \t ${CMAKE_INSTALL_PREFIX}")
message("Testing enabled:  \t ${ENABLE_TESTING}")
message("Clang-tidy:       \t ${ENABLE_CLANG_TIDY}")
message("Cppcheck:         \t ${ENABLE_CPPCHECK}")
message("Compiler:         \t ${CMAKE_CXX_COMPILER_ID}")
message("Sanizizers:       \t ${ENABLE_ASAN}")
message("Shared libs:      \t ${BUILD_SHARED_LIBS}")
message("Build libcpp:     \t ${LIBCPP}")
message("CCache executable:\t ${CCACHE}")
message("Building doc:     \t ${BUILD_DOC}")
message("------------------------------------------")
message("Version:          \t ${PROJECT_VERSION}")
message("########################################")

#-DCMAKE_TOOLCHAIN_FILE=C:/Users/occhi/Github/GBA-emulator/lib/vcpkg/scripts/buildsystems/vcpkg.cmake