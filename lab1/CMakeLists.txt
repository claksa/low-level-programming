cmake_minimum_required(VERSION 3.24)
project(llp)

set(CMAKE_CXX_STANDARD 17)

add_executable(llp element.h scheme_query.h scheme_query.cpp main.cpp element.cpp type.h type.cpp)
