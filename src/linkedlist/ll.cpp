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

class Node
{
private:
    Employee *employee;
    Node *next;

public:
    Node() : employee(NULL), next(NULL) {}
    ~Node() {}

    void setEmployee(Employee *e) { employee = e; }
    void setNext(Node *n) { next = n; }

    Employee *getEmployee() { return employee; }
    Node *getNext() { return next; }
};

class LinkedList
{
private:
    Node *head;
    Node *tail;
    int size;
    int max_id;

public:
    LinkedList() : head(NULL), tail(NULL), size(0) {}
    ~LinkedList() {}

    void push(Employee *e);
    void addToEnd(int salary, int department);
    void remove(int id);
    void update(int id, int salary, int department);
    void saveToCSV(const string &outputFilename);
};

void LinkedList::push(Employee *e)
{
    Node *node = new Node();
    node->setEmployee(e);
    node->setNext(NULL);

    if (head == NULL)
    {
        head = node;
        tail = node;
    }
    else
    {
        // Add the node to the end of the list
        tail->setNext(node);
        tail = node;
    }

    // Increase the size
    size++;
    max_id = e->getId();
}

void LinkedList::addToEnd(int salary, int department)
{
    // Create a new employee
    Employee *employee = new Employee();
    employee->setId(max_id + 1);
    employee->setSalary(salary);
    employee->setDepartment(department);

    // Create a new node
    Node *node = new Node();
    node->setEmployee(employee);
    node->setNext(NULL);

    // If the list is empty
    if (head == NULL)
    {
        head = node;
        tail = node;
    }
    else
    {
        // Add the node to the end of the list
        tail->setNext(node);
        tail = node;
    }

    // Increase the size
    size++;
    max_id++;
}

void LinkedList::remove(int id)
{
    Node *current = head;
    Node *previous = NULL;

    // Find the node to be deleted
    while (current != NULL && current->getEmployee()->getId() != id)
    {
        previous = current;
        current = current->getNext();
    }

    // If the node is found
    if (current != NULL)
    {
        // If the node is the head
        if (current == head)
        {
            head = head->getNext();
        }
        else
        {
            previous->setNext(current->getNext());
        }

        // If the node is the tail
        if (current == tail)
        {
            tail = previous;
        }

        // Delete the node
        delete current;

        // Decrease the size
        size--;
    }
    else
    {
        cout << "ERROR: An invalid ID to delete" << endl;
    }
}

void LinkedList::update(int id, int salary, int department)
{
    Node *current = head;

    // Find the node to be updated
    while (current != NULL && current->getEmployee()->getId() != id)
    {
        current = current->getNext();
    }

    // If the node is found
    if (current != NULL)
    {
        // Update the node
        current->getEmployee()->setSalary(salary);
        current->getEmployee()->setDepartment(department);
    }
    else
    {
        cout << "ERROR: An invalid ID to update" << endl;
    }
}

void LinkedList::saveToCSV(const string &outputFilename)
{
    ofstream file(outputFilename);

    // Write the keys
    file << "Employee_ID"
         << ";"
         << "Salary"
         << ";"
         << "Department" << endl; // Write headings

    // Write the employees
    Node *current = head;
    while (current != NULL)
    {
        file << current->getEmployee()->getId() << ";"
             << current->getEmployee()->getSalary() << ";"
             << current->getEmployee()->getDepartment() << endl;

        current = current->getNext();
    }

    file.close();
}

void loadFromCSV(const string &dataFilename, LinkedList &list)
{
    ifstream file(dataFilename);
    string line;

    string id, salary, department;

    getline(file, line);
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

            list.push(employee);
        }
    }

    file.close(); // Close the file
}

void processOperations(const string &operationsFilename, LinkedList &list)
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

            list.addToEnd(stoi(salary), stoi(department));
        }
        else if (operation == "UPDATE")
        {
            // cout << "UPDATE" << endl;
            getline(ss, id, ';');
            getline(ss, salary, ';');
            getline(ss, department, ';');

            list.update(stoi(id), stoi(salary), stoi(department));
        }
        else if (operation == "DELETE")
        {
            // cout << "DELETE" << endl;
            getline(ss, id, ';');
            list.remove(stoi(id));
        }
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
    LinkedList list;

    // Load the employees from the data file
    loadFromCSV(argv[1], list);

    // Process the operations
    processOperations(argv[2], list);

    // Save the employees to the data file
    list.saveToCSV("linked_list_solution.csv");

    return 0;
}