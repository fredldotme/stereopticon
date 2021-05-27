cmake_minimum_required(VERSION 3.00)

project(stereopticon-host LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

include(FindPkgConfig)
pkg_check_modules(SDL2 sdl2 REQUIRED)

include_directories(
    SYSTEM

    ${SDL2_INCLUDE_DIRS}
)

set(SRC
    main.cpp
)

add_executable(stereopticon-host
    ${SRC}
)

target_link_libraries(
    stereopticon-host

    ${SDL2_LDFLAGS}
    pthread
)

install(TARGETS stereopticon-host DESTINATION bin)
