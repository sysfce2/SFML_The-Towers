cmake_minimum_required(VERSION 3.15)

project(TheTowers
    VERSION 1.0.0
    LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Dependencies
add_subdirectory(deps/glfw)
add_subdirectory(deps/glm)

# Client sources
file(GLOB_RECURSE CLIENT_SOURCES CONFIGURE_DEPENDS
    src/Client/*.cpp
    src/Client/*.c
)

# Server sources
file(GLOB_RECURSE SERVER_SOURCES CONFIGURE_DEPENDS
    src/Server/*.cpp
    src/Server/*.c
)

# Targets
add_executable(client ${CLIENT_SOURCES})
add_executable(server ${SERVER_SOURCES})

# Client includes
target_include_directories(client PRIVATE
    src
    src/Client
    src/Client/Engine
    src/Client/Game
    src/Client/GUI
    src/Client/Input
)

# Server includes
target_include_directories(server PRIVATE
    src
)

# Client dependencies
target_link_libraries(client PRIVATE
    glfw
    glm
)
