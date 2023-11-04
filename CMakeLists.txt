cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)

set(PROJECT_NAME "r-dash")

project(
    ${PROJECT_NAME}
    VERSION 1.0.0
    DESCRIPTION "R-Dash"
    LANGUAGES CXX
)

add_executable(${PROJECT_NAME})

add_subdirectory(deps)

add_subdirectory(src)
