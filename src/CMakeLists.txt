cmake_minimum_required(VERSION 3.14)

project(stereopticon LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

include(FindPkgConfig)
pkg_check_modules(MIRSERVER mirserver>=0.26 REQUIRED)
pkg_check_modules(MIRRENDERERGLDEV mir-renderer-gl-dev>=0.26 REQUIRED)
pkg_check_modules(MIRAL miral>=2.0.0 REQUIRED)
pkg_check_modules(FREERDP freerdp2 REQUIRED)

include_directories(
    SYSTEM

    ${MIRSERVER_INCLUDE_DIRS}
    ${MIRRENDERERGLDEV_INCLUDE_DIRS}
    ${MIRAL_INCLUDE_DIRS}
    ${FREERDP_INCLUDE_DIRS}
)

set(SRC
    main.cpp
    window_manager.cpp
    window_manager.h
    rdpserver.cpp
    rdpserver.h
)

add_executable(stereopticon
    ${SRC}
)

target_link_libraries(
    stereopticon

    ${MIRAL_LDFLAGS}
    ${FREERDP_LDFLAGS}
    pthread
)

install(TARGETS stereopticon DESTINATION bin)
