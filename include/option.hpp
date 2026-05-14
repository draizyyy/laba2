#pragma once
#include "exceptions.hpp"

namespace myLib {

template<typename T>
class Option {
private:
    alignas(T) unsigned char storage[sizeof(T)];
    bool hasValue;

    T* ptr() {
        return static_cast<T*>(static_cast<void*>(&storage));
    }
    const T* ptr() const {
        return static_cast<const T*>(static_cast<const void*>(&storage));
    }

public:
    Option() : hasValue(false) {}

    Option(const T& value) : hasValue(true) {
        new (ptr()) T(value);
    }

    ~Option() {
        if (hasValue) {
            ptr()->~T();
        }
    }

    Option(const Option& other) : hasValue(other.hasValue) {
        if (hasValue) {
            new (ptr()) T(*other.ptr());
        }
    }

    Option& operator=(const Option& other) {
        if (this != &other) {
            if (hasValue) ptr()->~T();
            hasValue = other.hasValue;
            if (hasValue) {
                new (ptr()) T(*other.ptr());
            }
        }
        return *this;
    }

    bool HasValue() { 
        return hasValue;
    }
    bool IsNone() { 
        return !hasValue; 
    }

    T& GetValue() {
        if (!hasValue) throw EmptyCollectionException("Option пуст");
        return *ptr();
    }

    T GetValueOrDefault(T& def)  {
        return hasValue ? *ptr() : def;
    }
};

}