cmake_minimum_required(VERSION 3.10)
project(BallerEngine VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -g")
set(CMAKE_CXX_FLAGS_DEBUG "-g -DDEBUG")
set(CMAKE_EXPORT_COMPILE_COMMANDS True)

include_directories(include ${CMAKE_SOURCE_DIR}/bullet3/src)

set(SOURCE 
    src/controller.cpp
    src/camera.cpp
    src/gameObjects.cpp
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

set(SHADERS_SRC_DIR "${CMAKE_SOURCE_DIR}/src/shader")
set(SHADERS_DST_DIR "${CMAKE_BINARY_DIR}/shader")

add_custom_command(
    TARGET BallerEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${SHADERS_SRC_DIR} ${SHADERS_DST_DIR}
    COMMENT "Copying shaders directory to build directory..."
)

add_executable(physicsEngineTest tests/physicsEngineTest.cpp src/camera.cpp src/controller.cpp src/gameObjects.cpp)
add_executable(gameObjectsTest tests/gameObjectsTest.cpp src/camera.cpp src/controller.cpp src/gameObjects.cpp)

target_link_libraries(physicsEngineTest
    BulletDynamics 
    BulletCollision 
    LinearMath
    ${CMAKE_SOURCE_DIR}/lib/libraylib.so.5.5.0
    glfw GL X11 pthread Xrandr Xi dl
)
target_link_libraries(gameObjectsTest
    BulletDynamics 
    BulletCollision 
    LinearMath
    ${CMAKE_SOURCE_DIR}/lib/libraylib.so.5.5.0
    glfw GL X11 pthread Xrandr Xi dl
)

enable_testing()

add_test(NAME physicsEngineTest COMMAND physicsEngineTest)
add_test(NAME gameObjectsTest COMMAND gameObjectsTest)
