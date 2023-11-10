/*
@Author
StudentName : Seyyid Osman Sevgili
StudentID : 504221565
Date : 26.10.2023
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <time.h>

using namespace std;

// Employee class for the representing employees
class Employee {
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

Employee* loadFromCSV(const string &dataFilename, int &numEmployees) {
    ifstream file(dataFilename);
    string line;
    numEmployees = 0;

    // Find number of employees in the csv file to initialize the array
    getline(file, line);
    while (getline(file, line)) {
        numEmployees++;
    }

    // Seek to begining of the file again for reading data
    file.clear();  
    file.seekg(0);  

    // Create the array
    Employee* employees = new Employee[numEmployees];
    int index = 0;
    
    string id, salary, department;
    
    getline(file, line); // Skip the first line since these are only keys
    while (getline(file, line)) {
        stringstream ss(line);

        getline(ss, id, ';');
        getline(ss, salary, ';');
        getline(ss, department, ';');

        if (stoi(id) > 0){
            // Fill the employees array
            employees[index].setId(stoi(id));
            employees[index].setSalary(stoi(salary));
            employees[index].setDepartment(stoi(department));
            index++;
        }

    }

    file.close();  // Close the file
    return employees;  // Return employees array
}

Employee* addEmployee(Employee* employees, int &numEmployees, int salary, int department) {
    // Find the maximum id in the given data to find new employee id
    int id = 0;
    for (int i = 0; i < numEmployees; i++) {
        if (employees[i].getId() > id) {
            id = employees[i].getId();
        }
    }
    id++;  // Increment one the max id to get new id

    // Create new array since our capacity not enough for the adding new employee. Copy fiirst employees to the new array.
    Employee* extendedEmployees = new Employee[numEmployees + 1];
    for (int i = 0; i < numEmployees; i++) {
        extendedEmployees[i] = employees[i];
    }
    // Add new employee end of the new array
    extendedEmployees[numEmployees].setId(id);
    extendedEmployees[numEmployees].setSalary(salary);
    extendedEmployees[numEmployees].setDepartment(department);

    delete[] employees;  // Delete old array from the memory, since we do not need it further.
    numEmployees++;  
    return extendedEmployees;
}

void updateEmployee(Employee* employees, int numEmployees, int id, int salary, int department) {
    // Loop through the employees array to find given id. If there is no employee with given id give an error.
    for (int i = 0; i < numEmployees; i++) {
        if (employees[i].getId() == id) {
            employees[i].setSalary(salary);
            employees[i].setDepartment(department);
            return;
        }
    }
    cout << "ERROR: An invalid ID to update" << endl;
}

Employee* deleteEmployee(Employee* employees, int &numEmployees, int id) {
    int deleteIndex = -1; // Variable for the storing index that will be deleted
    // Loop through the employees for the finding index for the employee that will be deleted
    for (int i = 0; i < numEmployees; i++) {
        if (employees[i].getId() == id) {
            deleteIndex = i;
            break;
        }
    }
    
    // If there is no employee with this ID throw an error message
    if (deleteIndex == -1) {
        cout << "ERROR: An invalid ID to delete" << endl;
        return employees;  // End the function
    }

    // If id is valid, reduce the size of array
    Employee* newEmployees = new Employee[numEmployees - 1];
    int newIndex = 0; // Variable for the index mismatch
    for (int i = 0; i < numEmployees; i++) {
        if (i != deleteIndex) {
            newEmployees[newIndex] = employees[i];
            newIndex++;
        }
    }

    delete[] employees; // Delete the old array from memory
    numEmployees--; 
    return newEmployees;
}

void processOperations(const string &operationsFilename, Employee* &employees, int &numEmployees) {
    ifstream file(operationsFilename);
    string line;
    
    // Loop through operations
    while (getline(file, line)) {
        stringstream ss(line);
        string operation, id, salary, department;

        getline(ss, operation, ';');  // First of all pars the operation string

        // If it is ADD operation parse relevant data and apply it. Same as follows
        if (operation == "ADD") {
            getline(ss, salary, ';');
            getline(ss, department, ';');

            // cout << "ADD: " << salary << endl;

            employees = addEmployee(employees, numEmployees, stoi(salary), stoi(department));
        } 
        else if (operation == "UPDATE") {
            getline(ss, id, ';');
            getline(ss, salary, ';');
            getline(ss, department, ';');

            // cout << "Update ID: " << id << endl;

            updateEmployee(employees, numEmployees, stoi(id), stoi(salary), stoi(department));
        
        } 
        else if (operation == "DELETE") {
            getline(ss, id, ';');

            // cout << "Delete ID: " << id << endl;

            if (numEmployees <= 0) {
                cerr << "ERROR: There is no Employee" << endl;
                continue;
            }
            employees = deleteEmployee(employees, numEmployees, stoi(id));
        }
    }
    file.close();
}

void saveToCSV(const string &outputFilename, Employee* employees, int numEmployees) {
    ofstream file(outputFilename); // Create a output file

    file << "Employee_ID" << ";" << "Salary" << ";" << "Department" << endl;  // Write headings
    // Loop through the employees and write all of the to the csv
    for (int i = 0; i < numEmployees; i++) {
        file << employees[i].getId() << ";" << employees[i].getSalary() << ";" << employees[i].getDepartment() << endl;
    }

    file.close(); // Close the file
}

void printLastFiveEmployees(Employee* employees, int numEmployees){
    for (int i=numEmployees-5; i<numEmployees; i++){
        cout << employees[i].getId() << " " << employees[i].getSalary() << " " << employees[i].getDepartment() << endl;
    }
}

int main(int argc, char** argv) {
    if (argc != 3)
    {
        cerr << "Usage: " << argv[0] << " <data file> <operations file>" << endl;
        return 1;
    }
    int numEmployees = 0;
    // double exec_time;
    // clock_t start, end;

    // start = clock (); 
    Employee* employees = loadFromCSV(argv[1], numEmployees);

    // end = clock (); 
    // exec_time = ((double) (end - start)) / CLOCKS_PER_SEC;
    // cout << "loadFromCSV execution time in sec: " << exec_time << endl;

    // printLastFiveEmployees(employees, numEmployees);

    // start = clock (); 
    processOperations(argv[2], employees, numEmployees);

    // end = clock (); 
    // exec_time = ((double) (end - start)) / CLOCKS_PER_SEC;
    // cout << "processOperations execution time in sec: " << exec_time << endl;

    // printLastFiveEmployees(employees, numEmployees);

    // start = clock (); 
    saveToCSV("array_solution.csv", employees, numEmployees);

    // end = clock (); 
    // exec_time = ((double) (end - start)) / CLOCKS_PER_SEC;
    // cout << "saveToCSV execution time in sec:  " << exec_time << endl;

    // printLastFiveEmployees(employees, numEmployees);


    delete[] employees; // Free the memory


    return 0;
}
