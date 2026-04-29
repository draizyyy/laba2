#pragma once
#include "exceptions.hpp"

namespace myLib {

template<typename T>
class LinkedList {
private:
    struct Node {
        T value;
        Node* prev;
        Node* next;
        Node(T value, Node* prev, Node* next) : value(value), prev(prev), next(next) {}
        Node(T value) : value(value), prev(nullptr), next(nullptr) {}
        Node() : value(nullptr), prev(nullptr), next(nullptr) {}
    };
    Node* head;
    Node* tail;
    int size;

    Node* GetElement(int index) {
        Node* curr;
        if (index < (size / 2)) {
            curr = head;
            for (size_t i = 0; i < index; i++) {
                curr = curr->next;
            }
        } else {
            curr = tail;
            for (size_t i = size - 1; i > index; i--) {
                curr = curr->prev;
            }
        }
        return curr;
    }


public:
    LinkedList(T* items, int count) : LinkedList() {
        if (!items || count <= 0) {
            throw IndexOutOfRangeException();
        }
        for (size_t i = 0; i < count; i++) {
            this->Append(items[i]);
        }
    }
    LinkedList() : head(nullptr), tail(nullptr), size(0) {}
    LinkedList (const LinkedList <T> & list) : LinkedList() {
        Node* curr = list.head;
        while (curr) {
            Append(curr->value);
            curr = curr->next;
        }
    }

    ~LinkedList() {
        Node* curr = head;
        for (size_t i = 0; i < size; i++) {
            Node* next = curr->next;
            delete curr;
            curr = next;
        }
    }
    
    T GetFirst() {
        if (size == 0) {
            throw IndexOutOfRangeException();
        }
        return head->value;
    }

    T GetLast() {
        if (size == 0) {
            throw IndexOutOfRangeException();
        }
        return tail->value;
    }

    T Get(int index) {
        if (index < 0 || index >= size) {
            throw IndexOutOfRangeException();
        }
        return GetElement(index)->value;
    }

    LinkedList<T>* GetSubList(int startIndex, int endIndex) {
        if (startIndex < 0 || startIndex >= size || endIndex < 0 || endIndex >= size || startIndex > endIndex) {
            throw IndexOutOfRangeException();
        }
        LinkedList<T>* list = new LinkedList();
        Node* curr = GetElement(startIndex);
        for (size_t i = startIndex; i <= endIndex; i++) {
            list->Append(curr->value);
            curr = curr->next;
        }
        return list;
    }

    int GetLength() {
        return size;
    }


    void Append(T item) {
        Node* node = new Node(item);

        if (size == 0) {
            head = node;
            tail = node;
        } else {
            tail->next = node;
            node->prev = tail;
            tail = node;
        }
        size++;
    }
    
    void Prepend(T item) {
        Node* node = new Node(item);

        if (size == 0) {
            head = node;
            tail = node;
        } else {
            head->prev = node;
            node->next = head;
            head = node;
        }
        size++;
    }

    void InsertAt(T item, int index) {
        if (index < 0 || index > size) {
            throw IndexOutOfRangeException();
        }

        if (size == 0 || index == size) {
            Append(item);
        } else if (index == 0) {
            Prepend(item);
        } else {
            Node* node = new Node(item);
            Node* curr = GetElement(index);
            
            node->prev = curr->prev;
            node->next = curr;
            curr->prev->next = node;
            curr->prev = node;
            size++;
        }
    }

    LinkedList<T>* Concat(LinkedList<T>* list) {
        LinkedList<T>* newList = new LinkedList<T>();
        Node* curr = head;
        while (curr) {
            newList->Append(curr->value);
            curr = curr->next;
        }
        curr = list->head;
        while (curr) {
            newList->Append(curr->value);
            curr = curr->next;
        }
        return newList;
    }
};
}