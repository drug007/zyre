cmake_minimum_required(VERSION 3.7.2)
project(zink)
set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp zink.cpp)

add_subdirectory(.. zyre)

add_executable("${PROJECT_NAME}" ${SOURCE_FILES})

target_link_libraries("${PROJECT_NAME}"
    zyre
)