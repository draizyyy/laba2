#pragma once
#include "sequence.hpp"
#include "core/dynamic_array.hpp"
#include "core/linked_list.hpp"
#include <string>
#include <sstream>
#include <format>
#include "exceptions.hpp"
#include "sequence_iterator.hpp"

namespace myLib {

template<typename T>
class ArraySequence : public Sequence<T> {
protected:
    DynamicArray<T> data;
    virtual ArraySequence<T>* Clone() {
        return new ArraySequence<T>(data);
    }
    virtual ArraySequence<T>* Instance() {
        return this;
    }

    Sequence<T>* AppendDefault(T item, ArraySequence<T>* seq) {
        return InsertAtDefault(item, seq->GetLength(), seq);
    }

    Sequence<T>* PrependDefault(T item, ArraySequence<T>* seq) {
        return InsertAtDefault(item, 0, seq);
    }

    Sequence<T>* InsertAtDefault(T item, size_t index, ArraySequence<T>* seq) {
        size_t size = seq->GetLength();
        if (index > size) {
            throw IndexOutOfRangeException(index, size);
        }

        DynamicArray<T> arr(size + 1);

        for (size_t i = 0; i < index; i++) {
            arr.Set(i, seq->data.Get(i));
        }

        arr.Set(index, item);

        for (size_t i = index; i < size; i++) {
            arr.Set(i + 1, seq->data.Get(i));
        }

        seq->data = std::move(arr);
        return seq;
    }

    Sequence<T>* DeleteAtDefault(size_t index, ArraySequence<T>* seq) {
        size_t size = seq->GetLength();
        if (index >= size) {
            throw IndexOutOfRangeException(index, size);
        }

        if (size == 1) {
            seq->data = DynamicArray<T>();
            return seq;
        }

        DynamicArray<T> arr(size - 1);

        for (size_t i = 0; i < index; i++) {
            arr.Set(i, seq->data.Get(i));
        }

        for (size_t i = index + 1; i < size; i++) {
            arr.Set(i - 1, seq->data.Get(i));
        }

        seq->data = std::move(arr);
        return seq;
    }

    Sequence<T>* SetDefault(size_t index, T value, ArraySequence<T>* seq) {
        if (index >= seq->GetLength()) {
            throw IndexOutOfRangeException(index, seq->GetLength());
        }
        seq->data.Set(index, value);
        return seq;
    }

    Sequence<T>* ConcatDefault(Sequence<T>* list, ArraySequence<T>* seq) {
        if (!list || !seq) {
            throw InvalidInputException("Передан нулевой указатель");
        }

        DynamicArray<T> res(seq->data);

        size_t size = res.GetSize();
        res.Resize(size + list->GetLength());

        size_t i = size;

        for (const auto& el : *list) {
            res.Set(i++, el);
        }

        return new ArraySequence<T>(res);
    }

public:
    ArraySequence() = default;
    ArraySequence(T* items, size_t count) : data(items, count) {}
    ArraySequence(const DynamicArray<T>& list) : data(list) {}

    T& operator[](size_t index) {
        return data[index];
    }

    const T& operator[](size_t index) const {
        return data[index];
    }

    T GetFirst() override {
        return data.Get(0);
    }

    T GetLast() override {
        return data.Get(data.GetSize()-1);
    }

    T Get(size_t index) override {
        return data.Get(index);
    }
    Sequence<T>* GetSubsequence(size_t startIndex, size_t endIndex) override {
        DynamicArray<T> sub = data.SubArray(startIndex, endIndex);
        return new ArraySequence<T>(sub);
    }  
    size_t GetLength() override {
        return data.GetSize();
    }

    Sequence<T>* Append(T item) override {
        return AppendDefault(item, Instance());
    }

    Sequence<T>* Prepend(T item) override {
        return PrependDefault(item, Instance());
    }

    Sequence<T>* InsertAt(T item, size_t index) override {
        return InsertAtDefault(item, index, Instance());
    }

    Sequence<T>* DeleteAt(size_t index) override {
        return DeleteAtDefault(index, Instance());
    }

    Sequence<T>* Set(size_t index, T value) override {
        return SetDefault(index, value, Instance());
    }

    Sequence<T>* Concat(Sequence<T>* list) override {
        return ConcatDefault(list, Clone());
    }

    Sequence<T>* Empty() override {
        return new ArraySequence<T>();
    }

    SequenceIterator<T> begin() override {
        return data.begin();
    }

    SequenceIterator<T> end() override {
        return data.end();
    }

    template<typename T2>
    Sequence<T2>* Map(T2 (*func)(T)) {
        auto* result = new ArraySequence<T2>();

        for (const auto& val : *this) {
            result->Append(func(val));
        }

        return result;
    }

    Sequence<T>* Where(bool (*predicate)(T)) {
        auto* result = new ArraySequence<T>();

        for (const auto& val : *this) {
            if (predicate(val))
                result->Append(val);
        }

        return result;
    }
    
    template<typename T2>
    Sequence<T2>* Reduce(T2 (*func)(T2, T), T2 t2) {
        T2 t = t2;
        for (const auto& val : *this) {
            t = func(t, val);
        }
        auto* result = new ArraySequence<T2>();
        result->Append(t);
        return result;
    }
    template<typename T, bool IsPointer>
    struct StringConverter {
        static T parse(const std::string& s) {
            if (s.empty()) {
                throw InvalidInputException("Строка пустая");
            }
            std::istringstream iss(s);
            T value;
            iss >> value;
            if (iss.fail()) {
                throw InvalidInputException("Неверный формат данных");
            }
            return value;
        }
        static std::string format(T el) {
            return std::format("{}", el);
        }
    };

    template<typename T>
    struct StringConverter<T, true> {
        static T parse(const std::string& s) {
            throw InvalidInputException("Метод не поддерживается для указателей");
        }
        static std::string format(T el) {
            return "[указатель]";
        }
    };
    
    std::string elementToString(T el) {
        return StringConverter<T, std::is_pointer_v<T>>::format(el);
    }

    std::string ToString(T elem) override {
        return elementToString(elem);
    }

    std::string ToString() override {
        std::ostringstream oss;
        oss << "[";
        bool first = true;
        for (const auto& el : *this) {
            if (!first) oss << ", ";
            oss << elementToString(el);
            first = false;
        }
        oss << "]";
        return oss.str();
    }

    T FromString(const std::string& s) override {
        return StringConverter<T, std::is_pointer_v<T>>::parse(s);
    }
};

template<typename T>
class MutableArraySequence : public ArraySequence<T> {
    protected:
    ArraySequence<T>* Instance() override {
        return this;
    }
    public:
    using ArraySequence<T>::ArraySequence;
    Sequence<T>* Empty() override {
        return new MutableArraySequence<T>();
    }
};

template<typename T>
class ImmutableArraySequence : public ArraySequence<T> {
    protected:
    virtual ArraySequence<T>* Clone() override {
        return new ImmutableArraySequence<T>(this->data);
    }

    ArraySequence<T>* Instance() override {
        return Clone();
    }

    public:
    using ArraySequence<T>::ArraySequence;

    Sequence<T>* Empty() override {
        return new ImmutableArraySequence<T>();
    }

    Sequence<T>* Append(T item) override {
        return this->AppendDefault(item, this->Instance());
    }

    Sequence<T>* Prepend(T item) override {
        return this->PrependDefault(item, this->Instance());
    }

    Sequence<T>* InsertAt(T item, size_t index) override {
        return this->InsertAtDefault(item, index, this->Instance());
    }

    Sequence<T>* DeleteAt(size_t index) override {
        return this->DeleteAtDefault(index, this->Instance());
    }

    Sequence<T>* Concat(Sequence<T>* list) override {
        return this->ConcatDefault(list, this->Instance());
    }
};

}