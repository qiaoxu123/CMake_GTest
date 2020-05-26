cmake_minimum_required(VERSION 3.5)
project(CPP_TEST)

find_package(
    Gflag
)

set(CMAKE_BUILD_TYPE Release)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

include_directories(
    include
)

aux_source_directory(${PROJECT_SOURCE_DIR}/include DIR_SRCS)

add_executable(main src/main.cpp ${DIR_SRCS})
