cmake_minimum_required(VERSION 3.16)
project(lab)

set(CMAKE_CXX_STANDARD 17)

set(TOP_DIR ${CMAKE_SOURCE_DIR})
set(SRC_DIR ${TOP_DIR}/src)
set(TESTS_DIR ${TOP_DIR}/tests)
set(BENCHMARK_DIR ${TOP_DIR}/benchmark)

set(LIB_NAME tree)

set(SRC_LIST
        ${SRC_DIR}/Tree.hpp
        ${SRC_DIR}/TreeAdapters.hpp
        ${SRC_DIR}/TreeAdapters.hxx
        ${SRC_DIR}/TreeFactory.hpp
        ${SRC_DIR}/TreeCommands.hpp
        ${SRC_DIR}/TreeCommands.hxx
        ${SRC_DIR}/BTree.hpp
        ${SRC_DIR}/BTree.hxx
        ${SRC_DIR}/RedBlackTree.hpp
        ${SRC_DIR}/RedBlackTree.hxx
        )

add_library(${LIB_NAME} INTERFACE)

target_sources(${LIB_NAME} INTERFACE ${SRC_LIST})

target_include_directories (${LIB_NAME}
        INTERFACE
        $<BUILD_INTERFACE:${SRC_DIR}/${LIB_NAME}>
        $<INSTALL_INTERFACE:${LIB_NAME}>
        )

option(ENABLE_TESTS "Build tests for project" ON)
option(ENABLE_BENCHMARK "Build tests for project" ON)

if (ENABLE_TESTS)
    enable_testing()
    add_subdirectory(${TESTS_DIR})
endif()

if (ENABLE_BENCHMARK)
    add_subdirectory(${BENCHMARK_DIR})
endif()

# executable for quick testing in main.cpp
add_executable(main main.cpp benchmark/benchmark.cpp)
target_link_libraries(main PRIVATE ${LIB_NAME})
