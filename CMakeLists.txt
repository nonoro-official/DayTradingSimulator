cmake_minimum_required(VERSION 3.11) # FetchContent is available in 3.11+
project(example)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Dependencies
set(RAYLIB_VERSION 5.5)
find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
    include(FetchContent)
    FetchContent_Declare(
            raylib
            DOWNLOAD_EXTRACT_TIMESTAMP OFF
            URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    )
    FetchContent_GetProperties(raylib)
    if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
        set(FETCHCONTENT_QUIET NO)
        FetchContent_MakeAvailable(raylib)
        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
    endif()
endif()

# Our Project

add_executable(${PROJECT_NAME} src/main.cpp
        src/Objects/UI/Button.cpp
        src/Objects/UI/Button.h
        src/Objects/UI/Button.cpp
        src/Objects/UI/Button.h
        src/Objects/graph.cpp
        src/Objects/graph.h
        src/Objects/GenerateRandomMarket.cpp
        src/Objects/GenerateRandomMarket.h)
#set(raylib_VERBOSE 1)
target_link_libraries(${PROJECT_NAME} raylib)