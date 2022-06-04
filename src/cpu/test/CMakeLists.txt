# This file handles building each task as sub-project.
#
# Each task becomes an independent project in which you can experiment.
#
# Tasks are added as separate sub-projects so as compilation errors in
# one task don't affect another task.

cmake_minimum_required(VERSION 3.22)
project(GBA_emulator)

set(CMAKE_CXX_STANDARD 14)

add_executable(arm32_isa_test arm32_isa_test.cpp)
add_test(
        NAME arm32_isa_test
        COMMAND arm32_isa_test
)
add_executable(thumb_isa_test thumb_isa_test.cpp)
add_test(
        NAME thumb_isa_test
        COMMAND thumb_isa_test
)

# set(BINARY ${CMAKE_PROJECT_NAME}_tst)

# file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES false *.hh *.cpp)

# set(SOURCES ${TEST_SOURCES})

# add_executable(${BINARY} ${TEST_SOURCES} cpu/arm32_isa_test.cpp)

# add_test(NAME ${BINARY} COMMAND ${BINARY})

# target_link_libraries(${BINARY} PUBLIC ${CMAKE_PROJECT_NAME}_lib gtest)


# Add tasks subprojects
#include(cmake/utils.cmake)
#add_subprojects(${CMAKE_SOURCE_DIR})