#pragma once
#include "ienumerator.hpp"
#include "core/dynamic_array.hpp"
#include "exceptions.hpp"

namespace myLib {

template<typename T>
class ArrayEnumerator : public IEnumerator<T> {
private:
    DynamicArray<T>* data;
    int currentIndex;

public:
    explicit ArrayEnumerator(DynamicArray<T>* arr) : data(arr), currentIndex(-1) {}

    ~ArrayEnumerator() override = default;

    bool MoveNext() override {
        if (currentIndex + 1 < data->GetSize()) {
            currentIndex++;
            return true;
        }
        return false; 
    }

    T Current() override {
        if (currentIndex < 0 || currentIndex >= data->GetSize()) {
            throw IndexOutOfRangeException();
        }
        return data->Get(currentIndex);
    }

    void Reset() override {
        currentIndex = -1;
    }
};

} 