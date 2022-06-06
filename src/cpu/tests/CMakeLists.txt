add_executable(thumb_isa_test
        thumb_isa_test.cpp
        ../src/thumb_isa.cpp)

target_include_directories(${GBA_emulator}_test
        PUBLIC ../include
        PRIVATE ../src)

target_link_libraries(${GBA_emulator}_test
        PRIVATE
        project_options
        project_warnings
        PUBLIC)

include(CTest)
include(Catch)
catch_discover_tests(thumb_isa_test)