// #pragma once
// #include "person.hpp"
// #include <string>

// using namespace std;

// namespace myLib {

// class Student : public Person {
// private:
//     const string studentId;
//     string group;
//     const unsigned short yearOfEntry;

// public:
//     Student() : Person(), yearOfEntry(0) {}
    
//     Student(const PersonID& personId, const string& first, string& middle, string& last, const string& studentId, const string& group, int yearOfEntry)
//         : Person(personId, first, middle, last), studentId(studentId), group(group), yearOfEntry(yearOfEntry) {}
    
//     string GetStudentId() { 
//         return studentId; 
//     }
//     string GetGroup() {
//         return group; 
//     }
//     unsigned short GetYearOfEntry() {
//         return yearOfEntry; 
//     }
    
//     int GetCourse(unsigned short currentYear) {
//         int course = currentYear - yearOfEntry;
//         if (course >= 1 && course <= 4) {
//             return course;
//         } else {
//             return -1;
//         }
//     }
// };

// }