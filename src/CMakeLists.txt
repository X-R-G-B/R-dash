cmake_minimum_required(VERSION 3.20)

target_include_directories(
        ${PROJECT_NAME}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(
        ${PROJECT_NAME}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/GameSystems.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/CustomTypes.cpp
)

add_subdirectory(Map)
