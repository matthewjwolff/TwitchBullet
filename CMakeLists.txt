cmake_minimum_required(VERSION 3.8)
project(TwitchBullet)

add_executable(TwitchBullet main.cpp)

find_package(PkgConfig)
find_package(OpenGL)
find_package(glfw3)
find_package(Bullet)
find_package(glm)
pkg_search_module(glew glew)

target_link_libraries(TwitchBullet GL glfw BulletDynamics BulletCollision LinearMath GLEW)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
