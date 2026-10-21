cmake_minimum_required(VERSION 3.28)
project(CowboyRoguelike LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# SFML 3 is downloaded and built automatically, no manual installation needed
include(FetchContent)
# SFML 3.0.0 bundles dependencies that declare an old cmake_minimum_required, which CMake 4+ rejects
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
set(SFML_BUILD_AUDIO OFF)
set(SFML_BUILD_NETWORK OFF)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON
    SYSTEM)
FetchContent_MakeAvailable(SFML)

file(GLOB GAME_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
add_executable(game ${GAME_SOURCES})
target_link_libraries(game PRIVATE SFML::Graphics)

# Assets are loaded by relative path ("resources/..."), so run from the project folder
set_target_properties(game PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

# SFML is linked statically (no DLLs needed); put the resources next to the executable
add_custom_command(TARGET game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/resources $<TARGET_FILE_DIR:game>/resources)
