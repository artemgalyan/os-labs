cmake_minimum_required(VERSION 3.23)
project(demo_project)

set(CMAKE_CXX_STANDARD 20)

add_executable(demo_project main.cpp)

add_library(numlib STATIC IMPORTED)
set_target_properties(numlib PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/libnum.a)
set_target_properties(numlib PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/../number)

add_library(vectorlib SHARED IMPORTED)
set_target_properties(vectorlib PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/libvector.so)
set_target_properties(vectorlib PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/../vector)

target_link_libraries(demo_project numlib)
target_link_libraries(demo_project vectorlib)