/*
@Author
StudentName : Seyyid Osman Sevgili
StudentID : 504221565
Date : 09.11.2023
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <time.h>
#include <vector>

using namespace std;

// Employee class for the representing employees
class Employee
{
private:
    int id;
    int salary;
    int department;

public:
    Employee() : id(0), salary(0), department(0) {}
    ~Employee() {}

    void setSalary(int s) { salary = s; }
    void setId(int i) { id = i; }
    void setDepartment(int d) { department = d; }

    int getId() { return id; }
    int getSalary() { return salary; }
    int getDepartment() { return department; }
};

void loadFromCSV(const string &dataFilename, vector<Employee *> &employees, int &max_id)
{
    ifstream file(dataFilename);
    string line;

    string id, salary, department;

    getline(file, line); // Skip the first line since these are only keys
    while (getline(file, line))
    {
        stringstream ss(line);

        getline(ss, id, ';');
        getline(ss, salary, ';');
        getline(ss, department, ';');

        if (stoi(id) > 0)
        {
            Employee *employee = new Employee();
            employee->setId(stoi(id));
            employee->setSalary(stoi(salary));
            employee->setDepartment(stoi(department));

            employees.push_back(employee);

            if (stoi(id) > max_id)
            {
                max_id = stoi(id);
            }
        }
    }

    file.close(); // Close the file
}

void processOperations(const string &operationsFilename, vector<Employee *> &employees, int max_id)
{
    ifstream file(operationsFilename);
    string line;
    bool found;

    while (getline(file, line))
    {
        found = false;
        stringstream ss(line);

        string operation, id, salary, department;

        getline(ss, operation, ';'); // First of all pars the operation string

        if (operation == "ADD")
        {
            // cout << "ADD" << endl;
            getline(ss, salary, ';');
            getline(ss, department, ';');

            employees.push_back(new Employee());
            employees[employees.size() - 1]->setId(max_id + 1);
            employees[employees.size() - 1]->setSalary(stoi(salary));
            employees[employees.size() - 1]->setDepartment(stoi(department));

            max_id++;
        }
        else if (operation == "UPDATE")
        {
            // cout << "UPDATE" << endl;
            getline(ss, id, ';');
            getline(ss, salary, ';');
            getline(ss, department, ';');

            for (size_t i = 0; i < employees.size(); i++)
            {
                if (employees[i]->getId() == stoi(id))
                {
                    found = true;
                    employees[i]->setSalary(stoi(salary));
                    employees[i]->setDepartment(stoi(department));
                }
            }
            if (!found)
            {
                cout << "ERROR: An invalid ID to update" << endl;
            }
        }
        else if (operation == "DELETE")
        {
            // cout << "DELETE" << endl;
            getline(ss, id, ';');

            for (size_t i = 0; i < employees.size(); i++)
            {
                if (employees[i]->getId() == stoi(id))
                {
                    found = true;
                    employees.erase(employees.begin() + i);
                }
            }
            if (!found)
            {
                cout << "ERROR: An invalid ID to delete" << endl;
            }
        }
    }

    file.close(); // Close the file
}

void saveToCSV(const string &outputFileName, vector<Employee *> &employees)
{
    ofstream file(outputFileName);

    file << "Employee_ID"
         << ";"
         << "Salary"
         << ";"
         << "Department" << endl; // Write headings

    for (size_t i = 0; i < employees.size(); i++)
    {
        file << employees[i]->getId() << ";"
             << employees[i]->getSalary() << ";"
             << employees[i]->getDepartment() << endl;
    }

    file.close(); // Close the file
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cerr << "Usage: " << argv[0] << " <data file> <operations file>" << endl;
        return 1;
    }
    // Create a linked list and push the employees to the list
    vector<Employee *> employees;
    int max_id = 0;

    // Load the employees from the data file
    loadFromCSV(argv[1], employees, max_id);

    // Process the operations
    processOperations(argv[2], employees, max_id);

    // Save the employees to the data file
    saveToCSV("vector_solution.csv", employees);

    return 0;
}