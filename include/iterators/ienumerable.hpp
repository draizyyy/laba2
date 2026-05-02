#pragma once
#include "ienumerator.hpp"

namespace myLib {
    template <typename T>
    class IEnumerable {
    public:
        virtual ~IEnumerable() = default;
        virtual IEnumerator<T>* GetEnumerator() = 0;
    };
}