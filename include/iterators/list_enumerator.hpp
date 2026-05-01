#pragma once
#include "ienumerator.hpp"
#include "core/linked_list.hpp"
#include "exceptions.hpp"

namespace myLib {

template<typename T>
class ListEnumerator : public IEnumerator<T> {
private:
    LinkedList<T>* list;
    int currentIndex;

public:
    explicit ListEnumerator(LinkedList<T>* lst) : list(lst), currentIndex(-1) {}

    ~ListEnumerator() override = default;

    bool MoveNext() override {
        if (currentIndex + 1 < list->GetLength()) {
            currentIndex++;
            return true;
        }
        return false;
    }

    T Current() override {
        if (currentIndex < 0 || currentIndex >= list->GetLength()) {
            throw IndexOutOfRangeException();
        }
        return list->Get(currentIndex);
    }

    void Reset() override {
        currentIndex = -1;
    }
};

}