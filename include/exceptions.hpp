#pragma once
#include <stdexcept>
#include <string>

class IndexOutOfRangeException : public std::out_of_range {
public:
    IndexOutOfRangeException(const std::string& msg = "Неверный индекс") 
        : std::out_of_range(msg) {} 
};

class EmptyCollectionException : public std::runtime_error {
public:
    EmptyCollectionException(const std::string& msg = "Коллекция пуста") 
        : std::runtime_error(msg) {}
};