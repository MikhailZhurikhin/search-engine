cmake_minimum_required(VERSION 3.22)
project(tests)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(tests test_main.cpp
        ${MY_INCLUDE_DIR}/status.h
        ${MY_INCLUDE_DIR}/file_exchange.h
        ${MY_INCLUDE_DIR}/converter_json.h
        ${MY_SOURCE_DIR}/converter_json.cpp
        ${MY_INCLUDE_DIR}/inverted_index.h
        ${MY_SOURCE_DIR}/inverted_index.cpp
        ${MY_INCLUDE_DIR}/search_server.h
        ${MY_SOURCE_DIR}/search_server.cpp)



target_include_directories(tests PUBLIC ${MY_INCLUDE_DIR} ${MY_SOURCE_DIR})

include(FetchContent)
FetchContent_Declare(
        googletest
        URL
        https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
set(gtest_disable_pthreads on)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()

target_link_libraries(tests PRIVATE nlohmann_json::nlohmann_json gtest_main)

include(GoogleTest)
gtest_discover_tests(tests)

