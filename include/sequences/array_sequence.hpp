#pragma once
#include "sequence.hpp"
#include "core/dynamic_array.hpp"
#include "core/linked_list.hpp"
#include <string>
#include <sstream>
#include <format>
#include "exceptions.hpp"

namespace myLib {

template<typename T> struct is_ptr { 
    static constexpr bool value = false; 
};
template<typename T> struct is_ptr<T*> { 
    static constexpr bool value = true;
};

template<typename T>
class ArraySequence : public Sequence<T> {
protected:
    DynamicArray<T>* data;
    virtual ArraySequence<T>* Clone() {
        return new ArraySequence<T>(*data);
    }
    virtual ArraySequence<T>* Instance() {
        return this;
    }

    Sequence<T>* AppendDefault(T item, ArraySequence<T>* seq) {
        seq->data->InsertAt(item, seq->GetLength());
        return seq;
    }

    Sequence<T>* PrependDefault(T item, ArraySequence<T>* seq) {
        seq->data->InsertAt(item, 0);
        return seq;
    }

    Sequence<T>* InsertAtDefault(T item, size_t index, ArraySequence<T>* seq) {
        if (index > seq->GetLength()) {
            throw IndexOutOfRangeException(index, seq->GetLength());
        }
        seq->data->InsertAt(item, index);
        return seq;
    }

    // Sequence<T>* DeleteAtDefault(size_t index, ArraySequence<T>* seq) {
    //     if (index >= seq->GetLength()) {
    //         throw IndexOutOfRangeException(index, seq->GetLength());
    //     }
    //     seq->data->DeleteAt(index); 
    //     return seq;
    // }

    Sequence<T>* ConcatDefault(Sequence<T>* list, ArraySequence<T>* seq) {
        auto arr = static_cast<ArraySequence<T>*>(list); 
        DynamicArray<T>* res = seq->data->Concat(arr->data);
        Sequence<T>* result = new ArraySequence<T>(*res);
        delete res;
        return result;
    }
public:
    ArraySequence() : data(new DynamicArray<T>()) {};
    ArraySequence(T* items, size_t count) : data(new DynamicArray<T>(items, count)) {};
    ArraySequence(const DynamicArray<T>& list) : data(new DynamicArray<T>(list)) {}
    ~ArraySequence() override {
        delete data;
    }

    T GetFirst() override {
        return data->Get(0);
    }

    T GetLast() override {
        return data->Get(data->GetSize()-1);
    }

    T Get(size_t index) override {
        return data->Get(index);
    }
    Sequence<T>* GetSubsequence(size_t startIndex, size_t endIndex) override {
        DynamicArray<T>* sub = data->SubArray(startIndex, endIndex);
        Sequence<T>* seq = new ArraySequence<T>(*sub);
        delete sub;
        return seq;
    }  
    size_t GetLength() override {
        return data->GetSize();
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

    // Sequence<T>* DeleteAt(size_t index) override {
    //     return DeleteAtDefault(index, Instance());
    // }

    Sequence<T>* Concat(Sequence<T>* list) override {
        return ConcatDefault(list, Clone());
    }

    auto begin() const { 
        return data->begin();
    }
    auto end() const { 
        return data->end(); 
    }

        template<typename T2>
    Sequence<T2>* Map(T2 (*func)(T)) {
        size_t len = GetLength();
        T2* t2 = new T2[len];
        size_t i = 0;
        for (const auto& val : *this) {
            t2[i++] = func(val);
        }
        Sequence<T2>* seq = new ArraySequence<T2>(t2, len);
        delete[] t2;
        return seq;
    }

    Sequence<T>* Where(bool (*predicate)(T)) {
        T* t = new T[GetLength()];
        size_t count = 0;
        for (const auto& val : *this) {
            if (predicate(val)) {
                t[count++] = val;
            }
        }
        Sequence<T>* seq = new ArraySequence<T>(t, count);
        delete[] t;
        return seq;
    }

    template<typename T2>
    Sequence<T2>* Reduce(T2 (*func)(T2, T), T2 t2) {
        T2 t = t2;
        for (const auto& val : *this) {
            t = func(t, val);
        }
        T2 arr[1] = { t };
        return new ArraySequence<T2>(arr, 1);
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
};

template<typename T>
class ImmutableArraySequence : public ArraySequence<T> {
    protected:
    virtual ArraySequence<T>* Clone() override {
        return new ImmutableArraySequence<T>(*this->data);
    }

    ArraySequence<T>* Instance() override {
        return Clone();
    }

    public:
    using ArraySequence<T>::ArraySequence;

    Sequence<T>* Append(T item) override {
        return this->AppendDefault(item, this->Instance());
    }

    Sequence<T>* Prepend(T item) override {
        return this->PrependDefault(item, this->Instance());
    }

    Sequence<T>* InsertAt(T item, size_t index) override {
        return this->InsertAtDefault(item, index, this->Instance());
    }

    Sequence<T>* Concat(Sequence<T>* list) override {
        return this->ConcatDefault(list, this->Instance());
    }
};

}