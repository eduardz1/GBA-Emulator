# setting up sources
add_library(cpu_lib
        src/arm7tdmi.cpp
        src/thumb_isa.cpp
        src/bus.cpp
        src/arm32_isa.cpp)

# setting up include directories
target_include_directories(cpu_lib
        PUBLIC include
        PRIVATE src)

# linking against the desired targets
target_link_libraries(cpu_lib
        PRIVATE
        project_options
        project_warnings
        PUBLIC)

install(TARGETS cpu_lib
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${GBA_emulator}/
        RUNTIME DESTINATION       ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION       ${CMAKE_INSTALL_LIBDIR}
        INCLUDES DESTINATION      ${CMAKE_INSTALL_INCLUDEDIR}/${GBA_emulator}/
        ARCHIVE DESTINATION       ${CMAKE_INSTALL_SHAREDSTATEDIR})

if(ENABLE_TESTING)
    add_subdirectory(tests)
endif()