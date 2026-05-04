cmake_minimum_required(VERSION 3.14)
project(Laba2 CXX)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
  DOWNLOAD_EXTRACT_TIMESTAMP true
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(laba2_tests tests/tests.cpp)
target_include_directories(laba2_tests PRIVATE include)
target_link_libraries(laba2_tests PRIVATE gtest_main)

add_executable(laba2_app src/main.cpp)
target_include_directories(laba2_app PRIVATE include)


if(MSVC)
    target_compile_options(laba2_tests PRIVATE /wd4267)
    target_compile_options(laba2_app PRIVATE /wd4267)
endif()
