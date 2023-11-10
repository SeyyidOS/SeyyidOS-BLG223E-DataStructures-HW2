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
#include <list>

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

void loadFromCSV(const string &dataFilename, list<Employee *> &employees, int &max_id)
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

void processOperations(const string &operationsFilename, list<Employee *> &employees, int max_id)
{
    ifstream file(operationsFilename);
    string line;

    while (getline(file, line))
    {
        stringstream ss(line);

        string operation, id, salary, department;

        getline(ss, operation, ';'); // First of all pars the operation string

        if (operation == "ADD")
        {
            // cout << "ADD" << endl;
            getline(ss, salary, ';');
            getline(ss, department, ';');

            employees.push_back(new Employee());

            employees.back()->setId(max_id + 1);
            employees.back()->setSalary(stoi(salary));
            employees.back()->setDepartment(stoi(department));

            max_id++;
        }
        else if (operation == "UPDATE")
        {
            // cout << "UPDATE" << endl;
            getline(ss, id, ';');
            getline(ss, salary, ';');
            getline(ss, department, ';');

            for (list<Employee *>::iterator it = employees.begin(); it != employees.end(); it++)
            {
                if ((*it)->getId() == stoi(id))
                {
                    (*it)->setSalary(stoi(salary));
                    (*it)->setDepartment(stoi(department));
                }
            }
        }
        else if (operation == "DELETE")
        {
            // cout << "DELETE" << endl;
            getline(ss, id, ';');

            for (list<Employee *>::iterator it = employees.begin(); it != employees.end(); it++)
            {
                if ((*it)->getId() == stoi(id))
                {
                    employees.erase(it);
                    break;
                }
            }
        }
    }

    file.close(); // Close the file
}

void saveToCSV(const string &outputFileName, list<Employee *> &employees)
{
    ofstream file(outputFileName);

    file << "Employee_ID"
         << ";"
         << "Salary"
         << ";"
         << "Department" << endl; // Write headings

    for (list<Employee *>::iterator it = employees.begin(); it != employees.end(); it++)
    {
        file << (*it)->getId() << ";" << (*it)->getSalary() << ";" << (*it)->getDepartment() << endl;
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

    // double exec_time;
    // clock_t start, end;

    // start = clock ();

    list<Employee *> employees;
    int max_id = 0;

    loadFromCSV(argv[1], employees, max_id);

    processOperations(argv[2], employees, max_id);

    saveToCSV("list_solution.csv", employees);

    // end = clock ();
    // exec_time = ((double) (end - start)) / CLOCKS_PER_SEC;
    // cout << "Execution time in sec: " << exec_time << endl;

    return 0;
}