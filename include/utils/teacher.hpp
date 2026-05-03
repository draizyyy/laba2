#pragma once
#include "person.hpp"
#include <string>

using namespace std;

namespace myLib {

class Teacher : public Person {
private:
    string employeeId;
    string department;
    string post;
    int salary;

public:
    Teacher() : Person(), salary(0) {}
    
    Teacher(PersonID& personId, string& first, string& middle, string& last, string& employeeId,
        string& department, string& post, int salary)
        : Person(personId, first, middle, last),
          employeeId(employeeId), department(department), post(post), salary(salary) {}
    
    string GetEmployeeId() { 
        return employeeId;
    }
    string GetDepartment() { 
        return department;
    }
    string GetPost() { 
        return post;
    }
    int GetSalary() { 
        return salary;
    }
};

}