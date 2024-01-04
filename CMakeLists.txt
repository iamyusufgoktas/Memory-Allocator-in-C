cmake_minimum_required(VERSION 3.23)
project(MemoryAllocators)

set(CMAKE_CXX_STANDARD 23)

find_package(benchmark REQUIRED)

if(benchmark_FOUND)
    message("hello")
ENDIF()

add_executable(MemoryAllocators Freelistallocator.c)

target_link_libraries(MemoryAllocators benchmark::benchmark)


