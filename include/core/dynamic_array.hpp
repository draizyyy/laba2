#pragma once
#include "exceptions.hpp"
#include <iterator>
#include <cstring>
#include <initializer_list>

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
        std::memcpy(data, items, count * sizeof(T));
    }
    DynamicArray& operator=(const DynamicArray& other) {
        if (this != &other) {
            delete[] data;

            size = other.size;
            data = new T[size];

            if (size > 0) {
                std::memcpy(data, other.data, size * sizeof(T));
            }
        }
        return *this;
    }

    DynamicArray(DynamicArray&& other) : data(other.data), size(other.size) {
        other.data = nullptr;
        other.size = 0;
    }

    DynamicArray& operator=(DynamicArray&& other) noexcept {
        if (this != &other) {
            delete[] data;    
            
            data = other.data;  
            size = other.size;
            
            other.data = nullptr; 
            other.size = 0; 
        }
        return *this;
    }

    DynamicArray(std::initializer_list<T> list) : size(list.size()), data(new T[list.size()]) {
        if (size > 0) {
            std::memcpy(data, list.begin(), size * sizeof(T));
        }
    }
    
    T& operator[](size_t index) {
        if (index >= size)
            throw IndexOutOfRangeException(index, size);

        return data[index];
    }

    const T& operator[](size_t index) const {
        if (index >= size)
            throw IndexOutOfRangeException(index, size);

        return data[index];
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
        std::memcpy(data, other.data, size * sizeof(T));
    }
    ~DynamicArray() {
        delete[] data;
    }

    T& Get(size_t index) {
        if (index >= size) {
            throw IndexOutOfRangeException(index, size);
        }
        return data[index];
    }
    T Get(size_t index) const {
        if (index >= size) {
            throw IndexOutOfRangeException(index, size);
        }
        return data[index];
    }

    size_t GetSize() {
        return size;
    }
    DynamicArray<T>& Set(size_t index, T value) {
        if (index >= size) {
            throw IndexOutOfRangeException(index, size);
        }
        data[index] = value;

        return *this;
    }

    DynamicArray<T> SubArray(size_t start_index, size_t end_index) {
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
            return DynamicArray<T>();
        }

        size_t newSize = end_index - start_index;
        DynamicArray<T> res = DynamicArray<T>(newSize);
        
        std::memcpy(res.data, data + start_index, newSize * sizeof(T));

        return res;
    }


    // void InsertAt(T item, size_t index) {
    //     if (index > size) {
    //         throw IndexOutOfRangeException(index, size);
    //     }
    //     T* newData = new T[size + 1];
        
    //     if (index > 0) {
    //         std::memcpy(newData, data, index * sizeof(T));
    //     }
        
    //     newData[index] = item;

    //     if (index < size) {
    //         std::memcpy(newData + index + 1, data + index, (size - index) * sizeof(T));
    //     }

    //     delete[] data;
    //     data = newData;
    //     size++;
    // }

    // void Append(T item) {
    //     InsertAt(item, size);
    // }

    // void Prepend(T item) {
    //     InsertAt(item, 0);
    // }

    // void DeleteAt(size_t index) {
    //     if (index >= size) {
    //         throw IndexOutOfRangeException(index, size);
    //     }

    //     if (size == 1) {
    //         delete[] data;
    //         data = nullptr;
    //         size = 0;
    //         return;
    //     }

    //     T* newData = new T[size - 1];

    //     if (index > 0) {
    //         std::memcpy(newData, data, index * sizeof(T));
    //     }
        
    //     if (index < size - 1) {
    //         std::memcpy(newData + index, data + index + 1, (size - index - 1) * sizeof(T));
    //     }

    //     delete[] data;
    //     data = newData;
    //     size--;
    // }

    void Resize(size_t newSize) {
        T* newData = new T[newSize];
        size_t copySize = (size > newSize) ? newSize : size;

        if (copySize > 0) {
            std::memcpy(newData, data, copySize * sizeof(T));
        }

        delete[] data;
        data = newData;
        size = newSize;
    }
    
    DynamicArray<T>* Concat(DynamicArray<T>* arr) {
        if (!arr) {
            throw InvalidInputException("Передан нулевой указатель");
        }

        DynamicArray<T>* res = new DynamicArray<T>(size + arr->size);

        if (size > 0) {
            std::memcpy(res->data, data, size * sizeof(T));
        }
        
        if (arr->size > 0) {
            std::memcpy(res->data + size, arr->data, arr->size * sizeof(T));
        }

        return res;
    }
};

}