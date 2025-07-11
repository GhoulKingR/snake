cmake_minimum_required(VERSION 4.0)
project(snake)

add_subdirectory(lib)
add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} board snake-char ball)
