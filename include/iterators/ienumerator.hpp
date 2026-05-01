#pragma once

namespace myLib {

template<typename T>
class IEnumerator {
public:
    virtual ~IEnumerator() = default;
    
    virtual bool MoveNext() = 0;
    
    virtual T Current() = 0;

    virtual void Reset() = 0;
};

}