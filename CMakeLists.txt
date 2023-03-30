cmake_minimum_required(VERSION 3.22)

project(search_engine VERSION 1.0)

set(CMAKE_CXX_STANDARD 14)

set(JSON_BuildTests OFF CACHE INTERNAL "")

set(MY_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(MY_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(src)
add_subdirectory(nlohmann_json)
add_subdirectory(tests)

target_link_libraries(search_engine PRIVATE nlohmann_json::nlohmann_json)

configure_file( ${CMAKE_SOURCE_DIR}/app_version.h.in ${MY_INCLUDE_DIR}/app_version.h @ONLY)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
