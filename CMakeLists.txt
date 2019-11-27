cmake_minimum_required(VERSION 3.11)
project(byzantine C)

set(CMAKE_C_STANDARD 11)
add_executable(byzantine main.c byzantine.c byzantine.h)

find_package(MPI REQUIRED)
include_directories(SYSTEM ${MPI_INCLUDE_PATH})
target_link_libraries(byzantine ${MPI_C_LIBRARIES})

