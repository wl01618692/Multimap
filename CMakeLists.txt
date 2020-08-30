cmake_minimum_required(VERSION 3.15)
project(Program4)

find_package(Threads REQUIRED)
include(FetchContent)
set(CMAKE_CXX_STANDARD 17)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG master
)
FetchContent_GetProperties(googletest)
if (NOT googletest_POPULATED)
    FetchContent_Populate(googletest)
endif ()
set(CMAKE_CXX_STANDARD 17)
add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})

add_executable(Program4 LLRB-Multimap/multimap_tester.cc LLRB-Multimap/multimap.h)
target_compile_options(Program4 PRIVATE -Wall -Werror -Wextra)
target_link_libraries(Program4
        PRIVATE
        gtest
        gmock
        )