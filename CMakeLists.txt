cmake_minimum_required(VERSION 4.3.1)

project(vuz)

add_executable(vuz main.c utils.c map.c unit.c events.c bullet.c)

add_subdirectory(SDL2/)
target_link_libraries(vuz PRIVATE SDL2::SDL2 m)
