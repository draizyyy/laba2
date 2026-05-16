#pragma once
#include <stdexcept>
#include <string>
#include <format>

namespace myLib {

class IndexOutOfRangeException : public std::exception {
private:
    std::string message;
public:
    IndexOutOfRangeException(size_t invalid_index, size_t sequence_size) {
        message = std::format("Неверный индекс {}! Размер последовательности = {}, допустимы индексы от 0 до {}", 
            invalid_index, sequence_size, sequence_size - 1);
    }
    // IndexOutOfRangeException(size_t invalid_index) {
    //     message = std::format("Неверный индекс {}!", invalid_index);
    // }
    
    const char* what() const noexcept override {
        return message.c_str();
    }
};

class EmptyCollectionException : public std::exception {
private:
    std::string message;
public:
    EmptyCollectionException(const std::string& collection_name) {
        message = std::format("Коллекция {} пуста", collection_name);
    }
    
    const char* what() const noexcept override {
        return message.c_str();
    }
};

class InvalidInputException : public std::exception {
private:
    std::string message;
public:
    InvalidInputException(const std::string& error_description) {
        message = std::format("Неверный ввод: {}", error_description);
    }
    
    const char* what() const noexcept override {
        return message.c_str();
    }
};


}