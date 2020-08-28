cmake_minimum_required(VERSION 3.8)
project(TwitchBullet)

add_executable(TwitchBullet main.cpp)

find_package(PkgConfig)
find_package(OpenGL)
find_package(glfw3)
find_package(Bullet CONFIG REQUIRED)
find_package(glm)
pkg_search_module(glew glew)

include_directories(${BULLET_ROOT_DIR}/${BULLET_INCLUDE_DIR})


target_link_libraries(TwitchBullet GL glfw BulletDynamics BulletCollision LinearMath GLEW)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
