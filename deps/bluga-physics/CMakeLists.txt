cmake_minimum_required(VERSION 3.20)

include(FetchContent)

FetchContent_Declare(
    b-luga-physics
    URL "https://github.com/X-R-G-B/B-luga/releases/latest/download/b-luga-physics.tar"
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    DOWNLOAD_NO_PROGRESS FALSE
)

FetchContent_MakeAvailable(b-luga-physics)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    b-luga-physics
)
