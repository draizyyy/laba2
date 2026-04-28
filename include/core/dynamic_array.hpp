#pragma once
#include "exceptions.hpp"

namespace myLib {

template<typename T>
class DynamicArray {
private:
    T* data;
    int size;
public:
    DynamicArray(T* items, int count) : size(count) {
        data = new T[size];
        for (int i = 0; i < size; i++) {
            data[i] = items[i];
        }
    }
    DynamicArray(int size) : size(size) {
        data = new T[size]();
    }
    DynamicArray(const DynamicArray<T>& other) : size(other.size) {
        data = new T[size];
        for (int i = 0; i < size; i++) {
            data[i] = other.data[i];
        }
    }
    ~DynamicArray() {
        delete[] data;
    }
    T Get(int index) const {
        if (index < 0 || index >= size) throw IndexOutOfRangeException();
        return data[index];
    }
    int GetSize() const {
        return size;
    }
    void Set(int index, T value) {
        if (index < 0 || index >= size) throw IndexOutOfRangeException();
        data[index] = value;
    }
    void Resize(int newSize) {
        if (newSize < 0) throw IndexOutOfRangeException();
        T* newData = new T[newSize]();
        int copyCount = (newSize < size) ? newSize : size;
        for (int i = 0; i < copyCount; i++) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        size = newSize;
    }
};

}