cmake_minimum_required(VERSION 3.16)

project(boundflow LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

add_subdirectory(external/googletest)

add_executable(boundflow_tests
    tests/test_bounded_queue.cpp
)

target_include_directories(boundflow_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(boundflow_tests PRIVATE
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(boundflow_tests)






