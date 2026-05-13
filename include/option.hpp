#pragma once
#include "exceptions.hpp"

namespace myLib {

template<typename T>
class Option {
private:
    alignas(T) unsigned char storage[sizeof(T)];
    bool hasValue;

public:
    Option() : hasValue(false) {}

    Option(const T& value) : hasValue(true) {
        new (static_cast<T*>(static_cast<void*>(&storage))) T(value);
    }

    ~Option() {
        if (hasValue) {
            static_cast<T*>(static_cast<void*>(&storage))->~T();
        }
    }

    Option(const Option& other) : hasValue(other.hasValue) {
        if (hasValue) {
            new (static_cast<T*>(static_cast<void*>(&storage))) T(
                *static_cast<const T*>(static_cast<const void*>(&other.storage))
            );
        }
    }

    Option& operator=(const Option& other) {
        if (this != &other) {
            if (hasValue) static_cast<T*>(static_cast<void*>(&storage))->~T();
            hasValue = other.hasValue;
            if (hasValue) {
                new (static_cast<T*>(static_cast<void*>(&storage))) T(
                    *static_cast<const T*>(static_cast<const void*>(&other.storage))
                );
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
        return *static_cast<T*>(static_cast<void*>(&storage));
    }

    T GetValueOrDefault(T& def)  {
        return hasValue ? *static_cast<T*>(static_cast< void*>(&storage)) : def;
    }
};

}