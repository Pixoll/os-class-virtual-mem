cmake_minimum_required(VERSION 3.5)
project(os_class_virtual_mem)

set(CMAKE_CXX_STANDARD 17)

add_executable(
        mvirtual
        src/virtual_mem.cpp
        src/page_table.hpp
        src/read_references.hpp
)

add_executable(
        simulapc
        src/simulator.cpp
)
