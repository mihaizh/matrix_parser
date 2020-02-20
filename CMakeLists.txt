cmake_minimum_required(VERSION 3.10)
project(matrix_parser LANGUAGES CXX)

add_library(${PROJECT_NAME}
    include/matrix_parser.h
    src/matrix_parser.cpp)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        include)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO)

add_executable(${PROJECT_NAME}_test
    src/main.cpp)

target_link_libraries(${PROJECT_NAME}_test
    PRIVATE
        matrix_parser)
