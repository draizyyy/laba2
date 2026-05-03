#pragma once
#include "person.hpp"
#include <string>

using namespace std;

namespace myLib {

class Student : public Person {
private:
    string studentId;
    string group;
    int yearOfEntry;

public:
    Student() : Person(), yearOfEntry(0) {}
    
    Student(PersonID& personId, string& first, string& middle, string& last, const string& studentId, const string& group, int yearOfEntry)
        : Person(personId, first, middle, last), studentId(studentId), group(group), yearOfEntry(yearOfEntry) {}
    
    string GetStudentId() { 
        return studentId; 
    }
    string GetGroup() {
         return group; 
    }
    int GetYearOfEntry() {
         return yearOfEntry; 
    }
    
    int GetCourse(int currentYear) {
        int course = currentYear - yearOfEntry;
        if (course >= 1 && course <= 4) {
            return course;
        } else {
            return -1;
        }
    }
};

}