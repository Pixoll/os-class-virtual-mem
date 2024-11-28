cmake_minimum_required(VERSION 3.5)
project(os_class_virtual_mem)

set(CMAKE_CXX_STANDARD 17)

add_executable(
        mvirtual
        src/virtual_mem.cpp
        src/paging/page_table.hpp
        src/paging/optimal_page_table.hpp
        src/paging/fifo_page_table.hpp
        src/paging/lru_page_table.hpp
        src/paging/lru_clock_page_table.hpp
        src/paging/read_references.hpp
)

add_executable(
        simulapc
        src/simulator.cpp
        src/monitor_queue.hpp
)
