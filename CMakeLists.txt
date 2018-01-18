cmake_minimum_required(VERSION 3.7)
project(SPEC2017Events)

set(CMAKE_CXX_STANDARD 11)

set(events-handle events_handle.cpp)
add_executable(events-handle-exe ${events-handle})

set(events-table events_table.cpp)
add_executable(events-table-exe ${events-table})