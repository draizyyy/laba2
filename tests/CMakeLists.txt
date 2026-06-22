cmake_minimum_required(VERSION 3.14)

function(add_new_test test_name source_file)
    add_executable(${test_name} ${source_file})
    target_include_directories(${test_name} PRIVATE ${CMAKE_SOURCE_DIR}/include)
    
    target_link_libraries(${test_name} PRIVATE gtest_main)
    add_test(NAME ${test_name} COMMAND ${test_name})
endfunction()

add_new_test(test_linked_list test_linked_list.cpp)
add_new_test(test_dynamic_array test_dynamic_array.cpp)
add_new_test(test_sequences test_sequences.cpp)
add_new_test(test_bit_sequence test_bit_sequence.cpp)
add_new_test(test_option test_option.cpp)