#pragma once
#include "sequence.hpp"
#include "core/dynamic_array.hpp"
#include "core/linked_list.hpp"
#include "iterators/ienumerator.hpp"

namespace myLib {

template<typename T>
class ArraySequence : public Sequence<T> {
private:
    DynamicArray<T>* data;
protected:
    virtual ArraySequence<T>* Clone() {
        T* items = new T[data->GetSize()];
        for (int i = 0; i < data->GetSize(); i++) {
            items[i] = data->Get(i);
        }
        ArraySequence<T>* cloned = new ArraySequence<T>(items, data->GetSize());
        delete[] items;
        return cloned;
    };
    virtual ArraySequence<T>* Instance() {
        return this;
    };

    Sequence<T>* AppendDefault(T item, ArraySequence<T>* seq) {
        seq->data->Resize(seq->data->GetSize() + 1);
        seq->data->Set(seq->data->GetSize() - 1, item);
        return seq;
    }

    Sequence<T>* PrependDefault(T item, ArraySequence<T>* seq) {
        int oldSize = seq->data->GetSize();
        seq->data->Resize(oldSize + 1);
        for (int i = oldSize; i > 0; i--) {
            seq->data->Set(i, seq->data->Get(i - 1));
        }
        seq->data->Set(0, item);
        return seq;
    }

    Sequence<T>* InsertAtDefault(T item, int index, ArraySequence<T>* seq) {
        if (index < 0 || index > seq->data->GetSize()) {
            throw IndexOutOfRangeException();
        }
        int oldSize = seq->data->GetSize();
        seq->data->Resize(oldSize + 1);
        for (int i = oldSize; i > index; i--) {
            seq->data->Set(i, seq->data->Get(i - 1));
        }
        seq->data->Set(index, item);
        return seq;
    }

    Sequence<T>* ConcatDefault(Sequence<T>* list, ArraySequence<T>* seq) {
        int oldSize = seq->data->GetSize();
        seq->data->Resize(oldSize + list->GetLength());
        for (int i = 0; i < list->GetLength(); i++) {
            seq->data->Set(oldSize + i, list->Get(i));
        }
        return seq;
    }
public:
    ArraySequence() : data(new DynamicArray<T>(0)) {};
    ArraySequence(T* items, int count) : data(new DynamicArray(items, count)) {};
    ArraySequence(DynamicArray<T>& list) : data(nullptr) {
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
    }

    T GetFirst() override {
        return data->Get(0);
    }

    T GetLast() override {
        return data->Get(data->GetSize()-1);
    }

    T Get(int index) override {
        return data->Get(index);
    }
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
        return AppendDefault(item, Instance());
    }

    Sequence<T>* Prepend(T item) override {
        return PrependDefault(item, Instance());
    }

    Sequence<T>* InsertAt(T item, int index) override {
        return InsertAtDefault(item, index, Instance());
    }

    Sequence<T>* Concat(Sequence<T>* list) override {
        return ConcatDefault(list, Clone());
    }

    IEnumerator<T>* GetEnumerator() override {
        return new ArrayEnumerator<T>(data);
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

template<typename T>
class MutableArraySequence : public ArraySequence<T> {
    protected:
    ArraySequence<T>* Instance() override {
        return this;
    }
    public:
    using ArraySequence<T>::ArraySequence;
};

template<typename T>
class ImmutableArraySequence : public ArraySequence<T> {
    protected:
    ArraySequence<T>* Clone() override {
        int size = this->GetLength();
        T* items = new T[size];
        for (int i = 0; i < size; i++) {
            items[i] = this->Get(i);
        }
        ArraySequence<T>* cloned = new ImmutableArraySequence<T>(items, size);
        delete[] items;
        return cloned;
    }

    ArraySequence<T>* Instance() override {
        return Clone();
    }

    public:
    using ArraySequence<T>::ArraySequence;

    Sequence<T>* Append(T item) override {
        return AppendDefault(item, Instance());
    }

    Sequence<T>* Prepend(T item) override {
        return PrependDefault(item, Instance());
    }

    Sequence<T>* InsertAt(T item, int index) override {
        return InsertAtDefault(item, index, Instance());
    }

    Sequence<T>* Concat(Sequence<T>* list) override {
        return ConcatDefault(list, Instance());
    }
};

}