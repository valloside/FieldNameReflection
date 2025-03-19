cmake_minimum_required(VERSION 3.10.0)
project(FieldNameReflection VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)

include_directories(include)

file(GLOB_RECURSE native_srcs example/exapmle.cpp)
add_executable(exapmle ${native_srcs})
