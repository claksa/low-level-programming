cmake_minimum_required(VERSION 3.24)
project(llp)

set(CMAKE_CXX_STANDARD 17)

add_executable(llp element.h scheme.h main.cpp element.cpp type.h type.cpp database.h region.h meta_db_info.h collection.h)
