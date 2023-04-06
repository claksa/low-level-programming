cmake_minimum_required(VERSION 3.24)
project(llp)

set(CMAKE_CXX_STANDARD 17)

add_executable(lab1 document.h scheme.h main.cpp type.h type.cpp database.h region.h collection.h query.h)
