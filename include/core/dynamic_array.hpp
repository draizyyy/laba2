#pragma once
#include "exceptions.hpp"
#include <iterator>

namespace myLib {

template<typename T>
class DynamicArray {
private:
    T* data{};
    size_t size{}; 
public:
    class Iterator {
    private:
        T* current;

    public:
        using iterator_category = std::input_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        Iterator(T* curr) : current(curr) {}

        Iterator& operator++() { 
            ++current; 
            return *this;
        }
        Iterator operator++(int) { 
            Iterator iter = *this; 
            current++; 
            return iter; 
        }
        T& operator*() { 
            return *current; 
        }
        T* operator->() { 
            return current; 
        }

        bool operator==(const Iterator& other) const { 
            return current == other.current;
        }
        bool operator!=(const Iterator& other) const { 
            return !(*this == other); 
        }
    };

    DynamicArray() = default;
    DynamicArray(size_t count) : size(count), data(new T[count]) {}
    DynamicArray(T* items, size_t count) : DynamicArray(count) {
        if (!items && count > 0) {
            throw EmptyCollectionException("items");
        }
        for (size_t i = 0; i < count; i++) {
            data[i] = items[i];
        }
    }

    Iterator begin() const { 
        return Iterator(data); 
    }
    Iterator end() const { 
        return Iterator(data + size); 
    }

    DynamicArray(const DynamicArray& other) : DynamicArray(other.size) {
        if (size == 0) {
            return;
        }
        size_t i = 0;
        for (auto& el : other) {
            data[i++] = el;
        }
    }
    ~DynamicArray() {
        delete[] data;
    }
    T Get(size_t index) {
        if (index >= size) {
            throw IndexOutOfRangeException(index, size);
        }
        return data[index];
    }
    size_t GetSize() {
        return size;
    }
    void Set(size_t index, T value) {
        if (index >= size) {
            throw IndexOutOfRangeException(index, size);
        }
        data[index] = value;
    }

    DynamicArray<T>* SubArray(size_t start_index, size_t end_index) {
        if (start_index > size) {
            throw IndexOutOfRangeException(start_index, size);
        }
        if (end_index > size) {
            throw IndexOutOfRangeException(end_index, size);
        }
        if (start_index > end_index) {
            throw IndexOutOfRangeException(start_index, start_index-1);
        }
        if (start_index == end_index) {
            return new DynamicArray<T>();
        }

        DynamicArray<T>* res = new DynamicArray<T>(end_index - start_index);
        size_t i = 0;
        size_t curr = 0;
        for (auto& el : *this) {
            if (i >= start_index && i < end_index) {
                res->data[curr++] = el;
            }
            i++;
        }

        return res;
    }


    void InsertAt(T item, size_t index) {
        if (index > size) {
            throw IndexOutOfRangeException(index, size);
        }
        T* newData = new T[size + 1];
        
        size_t i = 0;
        for (auto& el : *this) {
            if (i < index) {
                newData[i] = el;
            } else {
                newData[i + 1] = el;
            }
            i++;
        }
        newData[index] = item;

        delete[] data;
        data = newData;
        size++;
    }

    // void Append(T item) {
    //     InsertAt(item, size);
    // }

    // void Prepend(T item) {
    //     InsertAt(item, 0);
    // }

    void DeleteAt(size_t index) {
        if (index >= size) {
            throw IndexOutOfRangeException(index, size);
        }

        if (size == 1) {
            delete[] data;
            data = nullptr;
            size = 0;
            return;
        }

        T* newData = new T[size - 1];
        size_t i = 0;
        size_t curr = 0;
        for (auto& el : *this) {
            if (i != index) {
                newData[curr++] = el;
            }
            i++;
        }

        delete[] data;
        data = newData;
        size--;
    }

    void Resize(size_t newSize) {
        T* newData = new T[newSize];
        size_t copySize = (size > newSize) ? newSize : size;
        size_t i = 0;
        for (auto& el : *this) {
            if (i >= copySize) {
                break;
            }
            newData[i++] = el;
        }
        delete[] data;
        data = newData;
        size = newSize;
    }
    
    DynamicArray<T>* Concat(DynamicArray<T>* array) {
        DynamicArray<T>* res = new DynamicArray<T>(size + array->size);
        size_t i = 0;
        for (auto& el : *this) {
            res->data[i++] = el;
        }
        size_t j = 0;
        for (auto& el : *array) {
            res->data[size + j++] = el;
        }
        return res;
    }
};

}