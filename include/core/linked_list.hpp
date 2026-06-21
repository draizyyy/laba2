#pragma once
#include "exceptions.hpp"
#include <iterator>

namespace myLib {
    
template<typename T>
class LinkedList {
private:
    struct Node {
        T value{};
        Node* prev{};
        Node* next{};
        Node() = default;
        Node(T value) : value(value) {}
        Node(T value, Node* prev, Node* next) : value(value), prev(prev), next(next) {}
    };
    Node* head{};
    Node* tail{};

public:
    class Iterator {
    private:
        Node* current;

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        Iterator(Node* node) : current(node) {}

        Iterator& operator++() { 
            if (current) current = current->next; 
            return *this; 
        }
        Iterator operator++(int) { 
            Iterator iter = *this; 
            if (current) {
                current = current->next; 
            }
            return iter; 
        }
        T& operator*() { 
            return current->value; 
        }
        T* operator->() { 
            return &current->value; 
        }

        bool operator==(const Iterator& other) const { 
            return current == other.current;
        }
        bool operator!=(const Iterator& other) const { 
            return !(*this == other); 
        }
    };

    Iterator begin() const { 
        return Iterator(head);
    }
    Iterator end() const { 
        return Iterator(nullptr); 
    }


    LinkedList() = default;
    LinkedList(T* items, size_t count) : LinkedList() {
        if (!items) {
            throw EmptyCollectionException("items");
        }
        for (size_t i = 0; i < count; i++) {
            Append(items[i]);
        }
    }
    LinkedList(const LinkedList<T>& list) : LinkedList() {
        for (auto& el : list) {
            Append(el);
        }
    }

    LinkedList& operator=(const LinkedList<T>& other) {
        if (this != &other) {
            while (head != nullptr) {
                Node* node = head;      
                head = head->next;    
                delete node;            
            }
            tail = nullptr;

            Node* curr = other.head;
            for (auto& el : other) {
                Append(el);
            }
        }
        return *this;
    }

    LinkedList(LinkedList&& other) : head(other.head), tail(other.tail) {
        other.head = nullptr;
        other.tail = nullptr;
    }

    LinkedList& operator=(LinkedList&& other) {
        if (this != &other) {
            while (head != nullptr) {
                Node* node = head;      
                head = head->next;    
                delete node;            
            }
            tail = nullptr;
            
            head = other.head;
            tail = other.tail;
            
            other.head = nullptr;
            other.tail = nullptr;
        }
        return *this;
    }

    ~LinkedList() {  
        while(head != nullptr) {
            Node* node = head;      
            head = head->next;    
            delete node;            
        }
    }
    
    T GetFirst() {
        if (!head) {
            throw EmptyCollectionException("LinkedList");
        }
        return head->value;
    }

    T GetLast() {
        if (!head) {
            throw EmptyCollectionException("LinkedList");
        }
        return tail->value;
    }

    T Get(size_t index) {
        if (index == 0) {
            return GetFirst();
        }
        if (!head) {
            throw EmptyCollectionException("LinkedList");
        }
        size_t i = 0;
        for (auto& el : *this) {
            if (i == index) {
                return el;
            }
            i++;
        }
        throw IndexOutOfRangeException(index, i-1);
    }

    LinkedList<T>* GetSubList(size_t startIndex, size_t endIndex) {
        if (startIndex > endIndex) {
            throw IndexOutOfRangeException(startIndex, startIndex-1);
        }
        if (!head) {
            throw EmptyCollectionException("LinkedList");
        }
        if (startIndex == endIndex) {
            return new LinkedList<T>();
        }

        LinkedList<T>* sublist = new LinkedList<T>();
        size_t index = 0;
        for (auto& el : *this) {
            if (index >= startIndex && index < endIndex) {
                sublist->Append(el);
            }
            index++;
            if (index >= endIndex) {
                break;
            }
        }
        
        if (index < endIndex) {
            throw IndexOutOfRangeException(endIndex, endIndex-1);
        }

        return sublist;
    }

    LinkedList(std::initializer_list<T> list) : LinkedList() {
        for (const auto& item : list) {
            Append(item);
        }
    }

    T& operator[](size_t index) {
        if (!head) {
            throw EmptyCollectionException("LinkedList");
        }
        Node* curr = head;
        for (size_t i = 0; i < index; i++) {
            if (!curr->next) {
                throw IndexOutOfRangeException(index, i);
            }
            curr = curr->next;
        }
        return curr->value;
    }

    const T& operator[](size_t index) const {
        if (!head) {
            throw EmptyCollectionException("LinkedList");
        }
        Node* curr = head;
        for (size_t i = 0; i < index; i++) {
            if (!curr->next) {
                throw IndexOutOfRangeException(index, i);
            }
            curr = curr->next;
        }
        return curr->value;
    }

    void Append(T item) {
        Node* node = new Node(item);
        if (!head) {
            head = node;
            tail = node;
        } else {
            tail->next = node;
            node->prev = tail;
            tail = node;
        }
    }
    
    void Prepend(T item) {
        Node* node = new Node(item);
        if (!head) {
            head = node;
            tail = node;
        } else {
            head->prev = node;
            node->next = head;
            head = node;
        }
    }

    void InsertAt(T item, size_t index) {
        if (index == 0) {
            Prepend(item);
            return;
        }
        if (!head) {
            throw IndexOutOfRangeException(index, 0);
        }

        Node* prevNode = head;
        for (size_t i = 1; i < index; i++) {
            if (!prevNode->next) {
                throw IndexOutOfRangeException(index, i-1);
            }
            prevNode = prevNode->next;
        }

        Node* node = new Node(item);

        node->next = prevNode->next;
        node->prev = prevNode;

        if (prevNode->next) {
            prevNode->next->prev = node;
        } else {
            tail = node; 
        }
        prevNode->next = node;
    }

    void DeleteAt(size_t index) {
        if (!head) {
            throw EmptyCollectionException("LinkedList");
        }

        Node* target = head;
        for (size_t i = 0; i < index; i++) {
            if (!target->next) {
                throw IndexOutOfRangeException(index, i);
            }
            target = target->next;
        }

        if (target->prev) {
            target->prev->next = target->next;
        } else {
            head = target->next;
        }

        if (target->next) {
            target->next->prev = target->prev;
        } else {
            tail = target->prev;
        }

        delete target;
    }

    LinkedList<T> Concat(const LinkedList<T>& list) {
        LinkedList<T> res(*this);

        for (const auto& el : list) {
            res.Append(el);
        }

        return res;
    }
};
}