cmake_minimum_required(VERSION 3.23)
project(vector)

set(CMAKE_CXX_STANDARD 20)

add_library(vector SHARED vector.cpp)

set(BUILD_DIR "cmake-build-debug")
set(NUMLIB_PROJECT_DIR ${CMAKE_SOURCE_DIR}/../number)
set(NUMLIB_BUILD_DIR ${NUMLIB_PROJECT_DIR}/${BUILD_DIR})

function(import_library LIBRARY_NAME LIBRARY_HEADER_PATH LIBRARY_BUILD_PATH)
    include_directories(${LIBRARY_HEADER_PATH})
    find_library(${LIBRARY_NAME}_LIBRARY ${LIBRARY_NAME} ${LIBRARY_BUILD_PATH})
    if(${LIBRARY_NAME}_LIBRARY)
        message("OK: ${${LIBRARY_NAME}_LIBRARY} found")
        target_link_libraries(${PROJECT_NAME} LINK_PUBLIC ${${LIBRARY_NAME}_LIBRARY})
    else()
        message(">>>> ${LIBRARY_NAME} not found!")
    endif()
endfunction()

import_library("num" ${NUMLIB_PROJECT_DIR} ${NUMLIB_BUILD_DIR})