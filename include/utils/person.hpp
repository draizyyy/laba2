#pragma once
#include <string>

using namespace std;

namespace myLib {

struct PersonID {
private:
    string series;
    string number;

public:
    PersonID() = default;
    PersonID(const string& s, const string& n) : series(s), number(n) {}
    
    string GetSeries() { 
        return series;
    }
    string GetNumber() { 
        return number;
    }
    string ToString() { 
        return series + " " + number;
    }
    
    bool operator==(PersonID& other) {
        return series == other.series && number == other.number;
    }
    bool operator!=(PersonID& other) { 
        return !(*this == other);
    }
    bool operator<(PersonID& other) {
        if (series != other.series) {
            return series < other.series;
        } else {
            return number < other.number;
        }
    }
};

class Person {
private:
    PersonID id;
    string firstName;
    string middleName;
    string lastName;

public:
    Person() {}
    
    Person(PersonID& personId, string& first, string& middle, string& last)
    : id(personId), firstName(first), middleName(middle), lastName(last) {}
    
    virtual ~Person() = default;

    PersonID GetID() { 
        return id;
    }
    string GetFirstName() { 
        return firstName;
    }
    string GetMiddleName() { 
        return middleName;
    }
    string GetLastName() { 
        return lastName;
    }

    string GetFullName() {
        return lastName + " " + firstName + " " + middleName;
    }
    
    bool operator==(Person& other) {
        return id == other.id;
     }
    bool operator!=(Person& other) { 
        return !(*this == other); 
    }
};

}