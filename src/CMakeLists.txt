# This file handles building each task as sub-project.
#
# Each task becomes an independent project in which you can experiment.
#
# Tasks are added as separate sub-projects so as compilation errors in
# one task don't affect another task.

cmake_minimum_required(VERSION 3.22)
project(GBA_emulator)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(cpu)
add_executable(main main.cpp)

# set(BINARY ${CMAKE_PROJECT_NAME})
# file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.hh *.cpp *.tpp)
# set(SOURCES ${SOURCES})
# add_executable(${BINARY}_run ${SOURCES})
# add_library(${BINARY}_lib STATIC ${SOURCES})


# Add tasks subprojects
#include(cmake/utils.cmake)
#add_subprojects(${CMAKE_SOURCE_DIR})