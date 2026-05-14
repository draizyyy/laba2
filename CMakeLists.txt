cmake_minimum_required(VERSION 3.14)
project(Laba2 CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# GoogleTest
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
  DOWNLOAD_EXTRACT_TIMESTAMP true
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# cpp-httplib — ОБРАТИ ВНИМАНИЕ: порядок и название в MakeAvailable
FetchContent_Declare(
  httplib
  URL https://github.com/yhirose/cpp-httplib/archive/refs/tags/v0.18.0.zip
  DOWNLOAD_EXTRACT_TIMESTAMP true
)
FetchContent_MakeAvailable(httplib)  # <-- было googletest, исправил на httplib

add_executable(laba2_tests tests/tests.cpp)
target_include_directories(laba2_tests PRIVATE include)
target_link_libraries(laba2_tests PRIVATE gtest_main)

add_executable(laba2_app src/main.cpp)
target_include_directories(laba2_app PRIVATE include)

# Подключение httplib
if(TARGET httplib::httplib)
    target_link_libraries(laba2_app PRIVATE httplib::httplib)
else()
    # Фоллбэк для header-only: вручную добавляем путь
    target_include_directories(laba2_app PRIVATE ${httplib_SOURCE_DIR})
endif()

# Windows: системные библиотеки
if(WIN32)
    target_link_libraries(laba2_app PRIVATE ws2_32 wsock32 crypt32)
endif()