cmake_minimum_required(VERSION 3.22)

add_executable(search_engine main.cpp
        ${MY_INCLUDE_DIR}/status.h
        ${MY_INCLUDE_DIR}/file_exchange.h
        ${MY_INCLUDE_DIR}/converter_json.h
                            converter_json.cpp
        ${MY_INCLUDE_DIR}/inverted_index.h
                            inverted_index.cpp
        ${MY_INCLUDE_DIR}/search_server.h
                            search_server.cpp)

target_include_directories(search_engine PUBLIC ${MY_INCLUDE_DIR})

