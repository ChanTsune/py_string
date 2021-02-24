cmake_minimum_required(VERSION 3.17)
project(py_string)

set(CMAKE_CXX_STANDARD 14)

add_library(py_string INTERFACE)
target_include_directories(py_string INTERFACE .)

add_executable(py_string_test test/test.cpp test/tupleplus.hpp)
find_package(Boost COMPONENTS system filesystem unit_test_framework REQUIRED)
target_link_libraries(py_string_test
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
        )

add_executable(py_string_example example.cpp)
