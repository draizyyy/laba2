#pragma once
#include "exceptions.hpp"
#include <utility>

namespace myLib {

struct NullOpt {};

namespace {
    constexpr NullOpt nullopt{};
}

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
    class Iterator {
    private:
        T* current;
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        Iterator(T* curr) : current(curr) {}

        Iterator& operator++() {
            if (current) {
                current++;
            }
            return *this;
        }
        Iterator operator++(int) {
            Iterator it = *this;
            if (current) {
                current++;
            }
            return it;
        }
        T& operator*() { 
            return *current;
        }
        T* operator->() { 
            return current; 
        }

        bool operator==(Iterator& other) { 
            return current == other.current; 
        }
        bool operator!=(Iterator& other) { 
            return current != other.current; 
        }
    };

    Iterator begin() {
        return Iterator(hasValue ? ptr() : nullptr);
    }
    Iterator end() {
        return Iterator(hasValue ? ptr() + 1 : nullptr);
    }


    Option() : hasValue(false) {}
    Option(NullOpt) : hasValue(false) {}
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

    Option(Option&& other) : hasValue(other.hasValue) {
        if (hasValue) {
            new (ptr()) T(std::move(*other.ptr()));
        }
    }

    Option& operator=(Option&& other) noexcept {
        if (this != &other) {
            if (hasValue) {
                ptr()->~T();
            }
            
            hasValue = other.hasValue;
            
            if (hasValue) {
                new (ptr()) T(std::move(*other.ptr()));
            }
        }
        return *this;
    }

    Option& operator=(NullOpt) {
        if (hasValue) {
            ptr()->~T();
        }
        hasValue = false;
        return *this;
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

    template <typename F>
    auto and_then(F&& f) && {
        using ReturnType = decltype(std::forward<F>(f)(std::move(*ptr())));
        if (!hasValue) {
            return ReturnType(nullopt);
        }
        return std::forward<F>(f)(std::move(*ptr()));
    }

    template <typename F>
    auto or_else(F&& f) && {
        if (hasValue) {
            return std::move(*this);
        }
        return std::forward<F>(f)();
    }


};

}