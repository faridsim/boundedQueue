cmake_minimum_required(VERSION 3.20)

project(boundflow)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

add_library(boundflow
    src/boundflow.cpp
)

add_executable(boundflow_smoke
    apps/boundflow_smoke.cpp
)

target_link_libraries(boundflow_smoke
    boundflow
)


