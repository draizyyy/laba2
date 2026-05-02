#pragma once
#include <exception>

namespace myLib {

template<typename T>
class Option {
private:
    T* value;
    bool hasValue;
public:
    Option() : value(nullptr), hasValue(false) {}
    Option(T value) : value(new T(value)), hasValue(true) {}
    ~Option() { delete value; }
    
    Option(const Option& other) : hasValue(other.hasValue) {
        value = hasValue ? new T(*other.value) : nullptr;
    }
    Option& operator=(const Option& other) {
        if (this != &other) {
            delete value;
            hasValue = other.hasValue;
            value = hasValue ? new T(*other.value) : nullptr;
        }
        return *this;
    }
    
    bool HasValue() { return hasValue; }
    bool IsNone() { return !hasValue; }
    
    T GetValue() const {
        if (!hasValue) throw std::exception();
        return *value;
    }
    T GetValueOrDefault(T def) { return hasValue ? *value : def; }
    
    static Option<T> None() { return Option<T>(); }
    static Option<T> Some(T value) { return Option<T>(value); }
};

}