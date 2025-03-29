cmake_minimum_required(VERSION 3.10)
project(BallerEngine VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -g")
set(CMAKE_CXX_FLAGS_DEBUG "-g -DDEBUG")
set(CMAKE_EXPORT_COMPILE_COMMANDS True)

include_directories(include ${CMAKE_SOURCE_DIR}/bullet3/src)

set(SOURCE 
    src/camera.cpp
    src/main.cpp
)

set(BULLET_BUILD_DIR ${CMAKE_BINARY_DIR}/bullet_build)

set(BUILD_BULLET2_DEMOS OFF CACHE BOOL "" FORCE)
set(BUILD_BULLET3 OFF CACHE BOOL "" FORCE)
set(BUILD_CPU_DEMOS OFF CACHE BOOL "" FORCE)
set(BUILD_EXTRAS OFF CACHE BOOL "" FORCE)
set(BUILD_UNIT_TESTS OFF CACHE BOOL "" FORCE)
set(INSTALL_LIBS ON CACHE BOOL "" FORCE)
add_subdirectory(bullet3 ${BULLET_BUILD_DIR})

add_executable(BallerEngine ${SOURCE})

target_link_libraries(BallerEngine
    BulletDynamics 
    BulletCollision 
    LinearMath
    ${CMAKE_SOURCE_DIR}/lib/libraylib.so.5.5.0
    glfw GL X11 pthread Xrandr Xi dl
)
