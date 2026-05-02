#pragma once
#include "core/linked_list.hpp"
#include "iterators/ienumerator.hpp"
#include "iterators/list_enumerator.hpp"
#include "option.hpp"

namespace myLib {
template<typename T>
class ListSequence : public Sequence<T> {
protected:
    LinkedList<T>* data;
public:
    ListSequence () : data(new LinkedList<T>()) {}
    ListSequence (T* items, int count) : data(new LinkedList<T>(items, count)) {}
    ListSequence(LinkedList<T>& list) : data(new LinkedList(list)) {}

    ~ListSequence() override {
        delete data;
    }

    T GetFirst() override {
        return data->GetFirst();
    }
    T GetLast() override {
        return data->GetLast();
    }
    T Get(int index) override {
        return data->Get(index);
    }

    Sequence<T>* GetSubsequence(int startIndex, int endIndex) override {
        Sequence<T>* seq = new ListSequence<T>(*data->GetSubList(startIndex, endIndex));
        return seq;
    }

    int GetLength() override {
        return data->GetLength();
    }

    Sequence<T>* Append(T item) override {
        data->Append(item);
        return this;
    }

    Sequence<T>* Prepend(T item) override {
        data->Prepend(item);
        return this;
    }

    Sequence<T>* InsertAt(T item, int index) override {
        data->InsertAt(item, index);
        return this;
    }

    Sequence<T>* Concat(Sequence<T>* list) override {
        LinkedList<T>* newList = new LinkedList<T>(*data);
        for (int i = 0; i < list->GetLength(); i++) {
            newList->Append(list->Get(i));
        }
        Sequence<T>* res = new ListSequence<T>(*newList);
        delete newList;
        return res;
    }
    IEnumerator<T>* GetEnumerator() override {
        return new ListEnumerator<T>(data);
    }

    template<typename T2>
    Sequence<T2>* Map(T2 (*func)(T)) {
        int len = GetLength();
        T2* t2 = new T2[len];
        for (int i = 0; i < len; i++) {
            t2[i] = func(Get(i));
        }
        Sequence<T2>* seq = new ListSequence<T2>(t2, len);
        delete[] t2;
        return seq;
    }

    Sequence<T>* Where(bool (*predicate)(T)) {
        int len = GetLength();
        T* t = new T[len];
        int count = 0;
        for (int i = 0; i < len; i++) {
            T t2 = Get(i);
            if (predicate(t2)) {
                t[count++] = t2;
            }
        }
        Sequence<T>* seq = new ListSequence<T>(t, count);
        delete[] t;
        return seq;
    }

    template<typename T2>
    Sequence<T2>* Reduce(T2 (*func)(T2, T), T2 t2) {
        T2 t = t2;
        int len = GetLength();
        for (int i = 0; i < len; i++) {
            t = func(t, Get(i));
        }
        return new ListSequence<T2>(&t, 1);
    }

    Option<T> GetFirst(bool (*predicate)(T)) {
        int len = GetLength();
        for (int i = 0; i < len; i++) {
            T t = Get(i);
            if (predicate == nullptr || predicate(t)) {
                return Option<T>(t);
            }
        }
        return Option<T>();
    }

    Option<T> GetLast(bool (*predicate)(T)) {
        int len = GetLength();
        for (int i = len - 1; i >= 0; i--) {
            T t = Get(i);
            if (predicate == nullptr || predicate(t)) {
                return Option<T>(t);
            }
        }
        return Option<T>();
    }
};
}