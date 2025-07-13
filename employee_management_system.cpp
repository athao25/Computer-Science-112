
/*
 * Employee Management System
 * This is my project for Computer Science 112
 * Author: Adam Thao
 * 
 * What this program does:
 * - You can login as different types of employees 
 * - HR people can add/delete/modify employees
 * - Managers can look at employee info
 * - Regular employees can only see their own info
 * - Uses classes and inheritance (pretty cool!)
 */

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <limits>

using namespace std;

// Employee class - this is the parent class for all employees
// Has basic info like name, ID, department, etc.
class Employee {
protected:
    string name;        // person's name
    int userId;         // their ID number
    string department;  // what department they work in
    string position;    // their job title
    double salary;      // how much money they make
    string userType;    // what kind of employee (HR, Management, or General)

public:
    // Constructor - this creates a new employee
    Employee(string n, int id, string dept, string pos, double sal, string type)
        : name(n), userId(id), department(dept), position(pos), salary(sal), userType(type) {}
    
    // Destructor - cleans up when employee is deleted
    virtual ~Employee() {}
    
    // These functions let you get the employee's info
    string getName() const { return name; }         // get name
    int getUserId() const { return userId; }        // get ID
    string getDepartment() const { return department; }  // get department
    string getPosition() const { return position; }      // get job title
    double getSalary() const { return salary; }          // get salary
    string getUserType() const { return userType; }      // get employee type
    
    // These functions let you change the employee's info
    void setName(const string& n) { name = n; }           // change name
    void setDepartment(const string& dept) { department = dept; }  // change department
    void setPosition(const string& pos) { position = pos; }        // change job title
    void setSalary(double sal) { salary = sal; }                   // change salary
    
    // Function to print out employee info
    virtual void displayInfo() const {
        cout << "\n--- Employee Information ---" << endl;
        cout << "Name: " << name << endl;
        cout << "User ID: " << userId << endl;
        cout << "Department: " << department << endl;
        cout << "Position: " << position << endl;
        cout << "Salary: $" << fixed << setprecision(2) << salary << endl;
        cout << "User Type: " << userType << endl;
    }
    
    // This function tells you what the employee can do in the system
    // Each type of employee will have different permissions
    virtual string getPermissions() const = 0;
};

// HR Employee class - these are the HR people who can do everything
// They inherit from the Employee class
class HREmployee : public Employee {
public:
    // Constructor for HR employee
    HREmployee(string n, int id, string dept, string pos, double sal)
        : Employee(n, id, dept, pos, sal, "HR") {}
    
    // HR people can do everything
    string getPermissions() const override {
        return "Full Access: Add, View, Search, Modify, Delete employees";
    }
    
    // Show HR employee info with their permissions
    void displayInfo() const override {
        Employee::displayInfo();
        cout << "Permissions: " << getPermissions() << endl;
    }
};

// Management Employee class - these are managers
// They can look at employee info but can't change anything
class ManagementEmployee : public Employee {
public:
    // Constructor for management employee
    ManagementEmployee(string n, int id, string dept, string pos, double sal)
        : Employee(n, id, dept, pos, sal, "Management") {}
    
    string getPermissions() const override {
        return "Limited Access: Search and View employees only";
    }
    
    void displayInfo() const override {
        Employee::displayInfo();
        cout << "Permissions: " << getPermissions() << endl;
    }
};

// General Employee class - regular workers
// They can only see their own information
class GeneralEmployee : public Employee {
public:
    // Constructor for regular employee
    GeneralEmployee(string n, int id, string dept, string pos, double sal)
        : Employee(n, id, dept, pos, sal, "General") {}
    
    string getPermissions() const override {
        return "Restricted Access: View own information only";
    }
    
    void displayInfo() const override {
        Employee::displayInfo();
        cout << "Permissions: " << getPermissions() << endl;
    }
};

// Main class that runs the whole program
// This handles login, menus, and all the employee operations
class EmployeeManagementSystem {
private:
    vector<Employee*> employees;  // list of all employees
    Employee* currentUser;        // who is logged in right now
    
    // Function to get a number from user and make sure it's valid
    int getValidInteger(const string& prompt) {
        int value;
        while (true) {
            cout << prompt;
            if (cin >> value) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');  // clear the input
                return value;
            } else {
                cout << "Invalid input. Please enter a number." << endl;
                cin.clear();  // fix the error
                cin.ignore(numeric_limits<streamsize>::max(), '\n');  // clear bad input
            }
        }
    }
    
    // Function to get a decimal number from user (like for salary)
    double getValidDouble(const string& prompt) {
        double value;
        while (true) {
            cout << prompt;
            if (cin >> value && value >= 0) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');  // clear the input
                return value;
            } else {
                cout << "Invalid input. Please enter a positive number." << endl;
                cin.clear();  // fix the error
                cin.ignore(numeric_limits<streamsize>::max(), '\n');  // clear bad input
            }
        }
    }
    
    // Function to get text from user
    string getStringInput(const string& prompt) {
        string input;
        cout << prompt;
        getline(cin, input);
        return input;
    }
    
    // Check if an employee ID is already being used
    bool userIdExists(int id) {
        for (const Employee* emp : employees) {
            if (emp->getUserId() == id) {
                return true;
            }
        }
        return false;
    }

public:
    // Constructor - sets up the system with some test employees
    EmployeeManagementSystem() : currentUser(nullptr) {
        // create some test employees to start with
        employees.push_back(new HREmployee("Sarah Johnson", 1001, "Human Resources", "HR Manager", 75000));
        
        // add a manager
        employees.push_back(new ManagementEmployee("Mike Davis", 2001, "Operations", "Operations Manager", 85000));
        
        // add some regular employees
        employees.push_back(new GeneralEmployee("John Smith", 3001, "IT", "Software Developer", 65000));
        employees.push_back(new GeneralEmployee("Emily Brown", 3002, "Marketing", "Marketing Specialist", 55000));
        employees.push_back(new GeneralEmployee("David Wilson", 3003, "Finance", "Financial Analyst", 60000));
    }
    
    // Destructor - cleans up memory when program ends
    ~EmployeeManagementSystem() {
        for (Employee* emp : employees) {
            delete emp;  // delete each employee to free memory
        }
    }
    
    // Function to log in a user
    bool login() {
        cout << "\n=== Employee Management System Login ===" << endl;
        int userId = getValidInteger("Enter your User ID: ");
        
        // Find user by ID
        // look through all employees to find the right ID
        for (Employee* emp : employees) {
            if (emp->getUserId() == userId) {
                currentUser = emp;  // remember who logged in
                cout << "\nLogin successful! Welcome, " << emp->getName() << endl;
                cout << "User Type: " << emp->getUserType() << endl;
                return true;
            }
        }
        
        cout << "Invalid User ID. Access denied." << endl;
        return false;
    }
    
    // Function to add a new employee (only HR can do this)
    void addEmployee() {
        if (currentUser->getUserType() != "HR") {
            cout << "Access denied. Only HR can add employees." << endl;
            return;
        }
        
        cout << "\n=== Add New Employee ===" << endl;
        
        string name = getStringInput("Enter employee name: ");
        
        int userId;
        // make sure the ID number isn't already used
        do {
            userId = getValidInteger("Enter unique User ID: ");
            if (userIdExists(userId)) {
                cout << "User ID already exists. Please choose a different ID." << endl;
            }
        } while (userIdExists(userId));
        
        string department = getStringInput("Enter department: ");
        string position = getStringInput("Enter position: ");
        double salary = getValidDouble("Enter salary: $");
        
        cout << "\nSelect employee type:" << endl;
        cout << "1. HR Employee" << endl;
        cout << "2. Management Employee" << endl;
        cout << "3. General Employee" << endl;
        
        int choice = getValidInteger("Enter choice (1-3): ");
        
        Employee* newEmployee = nullptr;
        
        // make the right type of employee based on what user picked
        switch (choice) {
            case 1:
                newEmployee = new HREmployee(name, userId, department, position, salary);
                break;
            case 2:
                newEmployee = new ManagementEmployee(name, userId, department, position, salary);
                break;
            case 3:
                newEmployee = new GeneralEmployee(name, userId, department, position, salary);
                break;
            default:
                cout << "Invalid choice. Creating as General Employee." << endl;
                newEmployee = new GeneralEmployee(name, userId, department, position, salary);
                break;
        }
        
        employees.push_back(newEmployee);
        cout << "\nEmployee added successfully!" << endl;
    }
    
    // Function to show employee information
    void viewEmployees() {
        if (currentUser->getUserType() == "General") {
            // General employees can only view their own information
            cout << "\n=== Your Employee Information ===" << endl;
            currentUser->displayInfo();
        } else {
            // HR and Management can view all employees
            cout << "\n=== All Employees ===" << endl;
            if (employees.empty()) {
                cout << "No employees found." << endl;
                return;
            }
            
            for (size_t i = 0; i < employees.size(); ++i) {
                cout << "\n--- Employee " << (i + 1) << " ---" << endl;
                employees[i]->displayInfo();
                cout << string(40, '-') << endl;
            }
        }
    }
    
    // Function to search for employees
    void searchEmployees() {
        if (currentUser->getUserType() == "General") {
            cout << "Access denied. General employees can only view their own information." << endl;
            return;
        }
        
        cout << "\n=== Search Employees ===" << endl;
        cout << "1. Search by User ID" << endl;
        cout << "2. Search by Name" << endl;
        cout << "3. Search by Department" << endl;
        
        int choice = getValidInteger("Enter search option (1-3): ");
        
        switch (choice) {
            case 1: {
                int searchId = getValidInteger("Enter User ID to search: ");
                bool found = false;
                for (Employee* emp : employees) {
                    if (emp->getUserId() == searchId) {
                        cout << "\n--- Search Result ---" << endl;
                        emp->displayInfo();
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    cout << "No employee found with User ID: " << searchId << endl;
                }
                break;
            }
            case 2: {
                string searchName = getStringInput("Enter name to search: ");
                bool found = false;
                // look for names that contain what the user typed
                for (Employee* emp : employees) {
                    if (emp->getName().find(searchName) != string::npos) {
                        cout << "\n--- Search Result ---" << endl;
                        emp->displayInfo();
                        found = true;
                    }
                }
                if (!found) {
                    cout << "No employee found with name containing: " << searchName << endl;
                }
                break;
            }
            case 3: {
                string searchDept = getStringInput("Enter department to search: ");
                bool found = false;
                for (Employee* emp : employees) {
                    if (emp->getDepartment().find(searchDept) != string::npos) {
                        cout << "\n--- Search Result ---" << endl;
                        emp->displayInfo();
                        found = true;
                    }
                }
                if (!found) {
                    cout << "No employee found in department: " << searchDept << endl;
                }
                break;
            }
            default:
                cout << "Invalid search option." << endl;
                break;
        }
    }
    
    // Function to change employee info (only HR can do this)
    void modifyEmployee() {
        if (currentUser->getUserType() != "HR") {
            cout << "Access denied. Only HR can modify employee information." << endl;
            return;
        }
        
        cout << "\n=== Modify Employee ===" << endl;
        int userId = getValidInteger("Enter User ID of employee to modify: ");
        
        Employee* employee = nullptr;
        for (Employee* emp : employees) {
            if (emp->getUserId() == userId) {
                employee = emp;
                break;
            }
        }
        
        if (!employee) {
            cout << "Employee not found with User ID: " << userId << endl;
            return;
        }
        
        cout << "\nCurrent employee information:" << endl;
        employee->displayInfo();
        
        cout << "\nWhat would you like to modify?" << endl;
        cout << "1. Name" << endl;
        cout << "2. Department" << endl;
        cout << "3. Position" << endl;
        cout << "4. Salary" << endl;
        
        int choice = getValidInteger("Enter choice (1-4): ");
        
        switch (choice) {
            case 1: {
                string newName = getStringInput("Enter new name: ");
                employee->setName(newName);
                cout << "Name updated successfully!" << endl;
                break;
            }
            case 2: {
                string newDept = getStringInput("Enter new department: ");
                employee->setDepartment(newDept);
                cout << "Department updated successfully!" << endl;
                break;
            }
            case 3: {
                string newPos = getStringInput("Enter new position: ");
                employee->setPosition(newPos);
                cout << "Position updated successfully!" << endl;
                break;
            }
            case 4: {
                double newSalary = getValidDouble("Enter new salary: $");
                employee->setSalary(newSalary);
                cout << "Salary updated successfully!" << endl;
                break;
            }
            default:
                cout << "Invalid choice." << endl;
                break;
        }
    }
    
    // Function to delete an employee (only HR can do this)
    void deleteEmployee() {
        if (currentUser->getUserType() != "HR") {
            cout << "Access denied. Only HR can delete employees." << endl;
            return;
        }
        
        cout << "\n=== Delete Employee ===" << endl;
        int userId = getValidInteger("Enter User ID of employee to delete: ");
        
        // Don't allow deletion of current user
        if (userId == currentUser->getUserId()) {
            cout << "Cannot delete your own account while logged in." << endl;
            return;
        }
        
        // find the employee with this ID (using fancy C++ stuff)
        auto it = find_if(employees.begin(), employees.end(),
                         [userId](Employee* emp) { return emp->getUserId() == userId; });
        
        if (it != employees.end()) {
            cout << "\nEmployee to be deleted:" << endl;
            (*it)->displayInfo();
            
            char confirm;
            cout << "\nAre you sure you want to delete this employee? (y/n): ";
            cin >> confirm;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // clear input
            
            if (confirm == 'y' || confirm == 'Y') {
                delete *it;         // delete the employee from memory
                employees.erase(it); // remove from the list
                cout << "Employee deleted successfully!" << endl;
            } else {
                cout << "Deletion cancelled." << endl;
            }
        } else {
            cout << "Employee not found with User ID: " << userId << endl;
        }
    }
    
    // Function to show the menu based on what type of user is logged in
    void displayMenu() {
        cout << "\n=== Main Menu ===" << endl;
        cout << "Logged in as: " << currentUser->getName() 
             << " (" << currentUser->getUserType() << ")" << endl;
        cout << string(40, '=') << endl;
        
        if (currentUser->getUserType() == "HR") {
            cout << "1. Add Employee" << endl;
            cout << "2. View All Employees" << endl;
            cout << "3. Search Employees" << endl;
            cout << "4. Modify Employee" << endl;
            cout << "5. Delete Employee" << endl;
            cout << "6. Logout" << endl;
        } else if (currentUser->getUserType() == "Management") {
            cout << "1. View All Employees" << endl;
            cout << "2. Search Employees" << endl;
            cout << "3. Logout" << endl;
        } else { // General Employee
            cout << "1. View My Information" << endl;
            cout << "2. Logout" << endl;
        }
    }
    
    // Main function that runs the whole program
    void run() {
        cout << "Welcome to the Employee Management Information System!" << endl;
        cout << "\nDefault Login Credentials for Testing:" << endl;
        cout << "HR User: 1001 (Sarah Johnson)" << endl;
        cout << "Management User: 2001 (Mike Davis)" << endl;
        cout << "General Employee: 3001 (John Smith), 3002 (Emily Brown), 3003 (David Wilson)" << endl;
        
        // keep trying to login until it works or user gives up
        while (!login()) {
            char retry;
            cout << "Would you like to try again? (y/n): ";
            cin >> retry;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // clear input
            
            if (retry != 'y' && retry != 'Y') {
                cout << "Goodbye!" << endl;
                return;
            }
        }
        
        // Main program loop
        while (true) {
            displayMenu();
            
            int choice;
            if (currentUser->getUserType() == "HR") {
                choice = getValidInteger("Enter your choice (1-6): ");
                
                switch (choice) {
                    case 1:
                        addEmployee();
                        break;
                    case 2:
                        viewEmployees();
                        break;
                    case 3:
                        searchEmployees();
                        break;
                    case 4:
                        modifyEmployee();
                        break;
                    case 5:
                        deleteEmployee();
                        break;
                    case 6:
                        cout << "Logging out... Goodbye!" << endl;
                        return;
                    default:
                        cout << "Invalid choice. Please try again." << endl;
                        break;
                }
            } else if (currentUser->getUserType() == "Management") {
                choice = getValidInteger("Enter your choice (1-3): ");
                
                switch (choice) {
                    case 1:
                        viewEmployees();
                        break;
                    case 2:
                        searchEmployees();
                        break;
                    case 3:
                        cout << "Logging out... Goodbye!" << endl;
                        return;
                    default:
                        cout << "Invalid choice. Please try again." << endl;
                        break;
                }
            } else { // General Employee
                choice = getValidInteger("Enter your choice (1-2): ");
                
                switch (choice) {
                    case 1:
                        viewEmployees();
                        break;
                    case 2:
                        cout << "Logging out... Goodbye!" << endl;
                        return;
                    default:
                        cout << "Invalid choice. Please try again." << endl;
                        break;
                }
            }
            
            // wait for user to press Enter before showing menu again
            cout << "\nPress Enter to continue...";
            cin.get();  // pause here
        }
    }
};

// Main function - this is where the program starts
int main() {
    try {
        EmployeeManagementSystem system;  // create the system
        system.run();  // start the program
    } catch (const exception& e) {
        cout << "An error occurred: " << e.what() << endl;  // something went wrong
        return 1;
    }
    
    return 0;
}