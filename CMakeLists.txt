cmake_minimum_required(VERSION 3.26)
project(klipper_16x4_display_design_tool)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

add_executable(klipper_16x4_display_design_tool main.cpp)

target_link_libraries(klipper_16x4_display_design_tool PRIVATE sfml-graphics sfml-audio)
target_compile_features(klipper_16x4_display_design_tool PRIVATE cxx_std_17)
target_compile_options(klipper_16x4_display_design_tool PRIVATE -fno-use-cxa-atexit)

add_custom_command(TARGET klipper_16x4_display_design_tool POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:klipper_16x4_display_design_tool> $<TARGET_FILE_DIR:klipper_16x4_display_design_tool> COMMAND_EXPAND_LISTS)

install(TARGETS klipper_16x4_display_design_tool)