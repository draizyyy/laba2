cmake_minimum_required(VERSION 3.14)
project(Laba2 CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC OFF)

set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/src/gui)

find_package(Qt5 REQUIRED COMPONENTS Widgets Core)

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

add_executable(laba2_gui WIN32
    src/main.cpp             
    src/SequenceWindow.cpp
    src/SequenceWindow.hpp
    src/gui/sequencewindow.ui
    src/sequencemodel.hpp
)

target_include_directories(laba2_gui PRIVATE include)
target_link_libraries(laba2_gui PRIVATE Qt5::Widgets Qt5::Core)