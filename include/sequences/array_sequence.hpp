#pragma once
#include "sequence.hpp"
#include "core/dynamic_array.hpp"
#include "core/linked_list.hpp"
#include "ienumerator.hpp"

namespace myLib {

template<typename T>
class ArraySequence : public Sequence<T> {
private:
    DynamicArray<T>* data;
public:
    ArraySequence() : data(new DynamicArray<T>(0)) {};
    ArraySequence(T* items, int count) : data(new DynamicArray(items, count)) {};
    ArraySequence(LinkedList<T>& list) : data(nullptr) {
        int size = list.GetLength();
        if (size == 0) {
            data = new DynamicArray<T>(0);
            return;
        }
        
        T* t = new T[size];
        for (int i = 0; i < size; i++) {
            t[i] = list.Get(i);
        }
        data = new DynamicArray<T>(t, size);
        delete[] t;
    }
    ~ArraySequence() override {
        delete data;
    };

    T GetFirst() override {
        return data->Get(0);
    };

    T GetLast() override {
        return data->Get(data->GetSize()-1);
    };

    T Get(int index) override {
        return data->Get(index);
    };
    Sequence<T>* GetSubsequence(int startIndex, int endIndex) override {
        int len = endIndex - startIndex + 1;
        T* t = new T[len]; 
        try {
            for (int i = 0; i < len; i++) {
                t[i] = data->Get(startIndex + i);
            }
            Sequence<T>* seq = new ArraySequence<T>(t, len); 
            delete[] t;
            return seq;
        } 
        catch (...) {
            delete[] t;
            throw;
        }
    }   
    int GetLength() override {
        return data->GetSize();
    }

    Sequence<T>* Append(T item) override {
        data->Resize(data->GetSize() + 1);
        data->Set(data->GetSize() - 1, item);
        return this;
    }

    Sequence<T>* Prepend(T item) override {
        int oldSize = data->GetSize();
        data->Resize(oldSize + 1);
        for (int i = oldSize; i > 0; i--) {
            data->Set(i, data->Get(i - 1));
        }
        data->Set(0, item);
        return this;
    }

    Sequence<T>* InsertAt(T item, int index) override {
        if (index < 0 || index > data->GetSize()) {
            throw IndexOutOfRangeException();
        }
        int oldSize = data->GetSize();
        data->Resize(oldSize + 1);
        for (int i = oldSize; i > index; i--) {
            data->Set(i, data->Get(i - 1));
        }
        data->Set(index, item);
        return this;
    }

    Sequence<T>* Concat(Sequence<T>* list) override {
        int len = data->GetSize() + list->GetLength();
        T* t = new T[len];
        
        for (int i = 0; i < data->GetSize(); i++) {
            t[i] = data->Get(i);
        }
        for (int i = 0; i < list->GetLength(); i++) {
            t[data->GetSize() + i] = list->Get(i);
        }
        
        Sequence<T>* seq = new ArraySequence<T>(t, len);
        delete[] t;
        return seq;
    }

    // template<typename T2>
    // Sequence<T2>* Map(T2 (*func)(T)) override;
    // Sequence<T>* Where(bool (*predicate)(T)) override;
    // template<typename T2>
    // T2 Reduce(T2 (*func)(T2, T), T2 seed) override;
    // Option<T> GetFirst(bool (*predicate)(T) = nullptr) override;
    // Option<T> GetLast(bool (*predicate)(T) = nullptr) override;
    // IEnumerator<T>* GetEnumerator() override;
};

}