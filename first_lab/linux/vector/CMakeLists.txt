cmake_minimum_required(VERSION 3.23)
project(vector)

set(CMAKE_CXX_STANDARD 20)

add_library(vector SHARED vector.cpp)

add_library(numlib STATIC IMPORTED)
set_target_properties(numlib PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/libnum.a)
set_target_properties(numlib PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/../number)

target_link_libraries(vector numlib)