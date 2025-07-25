cmake_minimum_required(VERSION 3.11) # FetchContent is available in 3.11+
project(DayTradingSimulator)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Dependencies
# raylib
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

# raygui
include(FetchContent)
FetchContent_Declare(
        raygui
        GIT_REPOSITORY https://github.com/raysan5/raygui.git
        GIT_TAG master
)
FetchContent_MakeAvailable(raygui)

# Our Project
add_executable(${PROJECT_NAME}
        src/Libraries/raygui_wrapper.cpp
        src/main.cpp
        src/Objects/Graph.cpp
        src/Objects/Graph.h
        src/Objects/GenerateRandomMarket.cpp
        src/Objects/GenerateRandomMarket.h
        src/Objects/Menu.cpp
        src/Objects/Menu.h
        src/GameState.cpp
        src/GameState.h
        src/Upgrades/Upgrade.cpp
        src/Upgrades/Upgrade.h
        src/Upgrades/UpgradeHandler.cpp
        src/Upgrades/UpgradeHandler.h
        src/Upgrades/UpgradeBuilder.cpp
        src/Upgrades/UpgradeBuilder.h
        src/Classes/PlayerData.cpp
        src/Classes/PlayerData.h
        src/Classes/Stock.cpp
        src/Classes/Stock.h
        src/Classes/Company.cpp
        src/Classes/Company.h
        src/Classes/MarketData.h
        src/Objects/MonthDisplay.cpp
        src/Objects/MonthDisplay.h
        src/Objects/ToggleButton.cpp
        src/Objects/ToggleButton.h
)

# Include paths
target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/src
        ${raygui_SOURCE_DIR}/src
)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

file(COPY ${CMAKE_SOURCE_DIR}/Textures DESTINATION ${CMAKE_BINARY_DIR})