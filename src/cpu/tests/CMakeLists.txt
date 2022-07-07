
target_include_directories(${GBA_emulator}_test
        PUBLIC ../include
        PRIVATE ../src)

target_link_libraries(${GBA_emulator}_test
        PRIVATE
        project_options
        project_warnings
        PUBLIC
        GTest::GTest
        GTest::Main)

# need to look at this 
add_executabe(${GBA_emulator}_test
        ${GBA_emulator}_test.cpp)
        
# need to look at this 
add_test(thumb_isa_test
        thumb_isa_test.cpp
        ../src/thumb_isa.cpp)
