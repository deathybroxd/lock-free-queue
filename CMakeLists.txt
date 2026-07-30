cmake_minimum_required(VERSION 3.16)
project(lfq LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_library(ringbuffer INTERFACE)
target_include_directories(ringbuffer INTERFACE include)

add_executable(lfq main.cpp)
target_link_libraries(lfq PRIVATE ringbuffer)


# make run
add_custom_target(run 
    COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/lfq
    DEPENDS lfq
)

# make val
add_custom_target(val
    COMMAND valgrind ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/lfq
    DEPENDS lfq
)


enable_testing()

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_subdirectory(tests)