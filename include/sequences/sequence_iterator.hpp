#pragma once

namespace myLib {

template<typename T>
class SequenceIterator {
    struct Base {
        virtual ~Base() = default;

        virtual T& get() = 0;
        virtual void next() = 0;
        virtual bool equals(const Base*) const = 0;
        virtual Base* clone() const = 0;
    };

    template<typename Iter>
    struct Impl : Base {
        Iter iter;

        Impl(Iter it) : iter(it) {}

        T& get() override {
            return *iter;
        }

        void next() override {
            ++iter;
        }

        bool equals(const Base* other) const override {
            auto p = dynamic_cast<const Impl*>(other);
            return p && iter == p->iter;
        }

        Base* clone() const override {
            return new Impl(iter);
        }
    };

    Base* ptr;

public:
    template<typename Iter>
    SequenceIterator(Iter it)
        : ptr(new Impl<Iter>(it))
    {}

    SequenceIterator(const SequenceIterator& other)
        : ptr(other.ptr->clone())
    {}

    ~SequenceIterator() {
        delete ptr;
    }

    T& operator*() {
        return ptr->get();
    }

    SequenceIterator& operator++() {
        ptr->next();
        return *this;
    }

    bool operator!=(const SequenceIterator& other) const {
        return !ptr->equals(other.ptr);
    }
};

}