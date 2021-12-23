// Name: Diego Bravo Torres
// ID : 138350202
// Date : August 6, 2021
// Purpose : Project Assignment
// Team Size : 1


#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
#include <occi.h>


using oracle::occi::Environment;
using oracle::occi::Connection;
using namespace oracle::occi;
using namespace std;


// Class definition
struct Employee {
    int employeeNumber;
    char lastName[50];
    char firstName[50];
    char extension[10];
    char email[100];
    char officecode[10];
    int reportsTo;
    char jobTitle[50];
};


// Menu interface
int menu(void) {

    int option = 0;
    int flag = 1;

    while (flag) {
        cout << endl <<"********************* HR Menu *********************" << endl;
        cout << "1) Find Employee" << endl;
        cout << "2) Employees Report" << endl;
        cout << "3) Add Employee" << endl;
        cout << "4) Update Employee" << endl;
        cout << "5) Remove Employee" << endl;
        cout << "0) Exit" << endl;
        cout << "Enter an option (0-5): ";
        cin >> option;

        if (option >= 0 && option <= 5)
            flag = 0;
    }

    return option;
}

 // Find Employee (Option 1)

int findEmployee(Connection* conn, int employeeNumber, struct Employee* emp) 
{
    int returnValue = 0;
    Statement* stmt = nullptr;
    stmt = conn->createStatement();


    try {
        
        // Convert employee number to a string
        string Snum = to_string(employeeNumber);

        // Build the statement
        string statement = "SELECT * FROM employees WHERE employeenumber = " + Snum;
        ResultSet* rs = stmt->executeQuery(statement);

        // Assing values from the Result set to the Emp struct
        while (rs->next())
        {
            returnValue = 1;
            emp->employeeNumber = rs->getInt(1);

            string lastName = rs->getString(2);
            strcpy(emp->lastName,lastName.c_str());

            string firstName = rs->getString(3);
            strcpy(emp->firstName, firstName.c_str());

            string extension = rs->getString(4);
            strcpy(emp->extension, extension.c_str());

            string email = rs->getString(5);
            strcpy(emp->email, email.c_str());

            string officeCode = rs->getString(6);
            strcpy(emp->officecode, officeCode.c_str());


            emp->reportsTo = rs->getInt(7);

            string jobTitle = rs->getString(8);
            strcpy(emp->jobTitle, jobTitle.c_str());

        }

    }
    catch (SQLException& sqlExcp) {
        cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
    }

    return returnValue;
}


// Employees Report (Option 2)

void displayAllEmployees(Connection* conn)
{

    Statement* stmt = nullptr;
    stmt = conn->createStatement();

    try {

        // Build the statement
        string statement = "SELECT employeenumber, lastname, firstname, extension, email, phone, reportsto FROM employees INNER JOIN offices ON employees.officecode = offices.officecode " ;
        ResultSet* rs = stmt->executeQuery(statement);


        // Print the header
        cout << "------   ---------------   -------------------------------   ------------------   ---------   --------------" << endl;
        cout << "ID       Employee Name     Email                             Phone                Extension   Manager Name  " << endl;
        cout << "------   ---------------   -------------------------------   ------------------   ---------   --------------" << endl;

        while (rs->next())
        {
            string Name = "";

            cout.width(9);
            cout << std::left << rs->getInt(1) ; // Print ID
            cout.width(18);

            Name.append(rs->getString(3));
            Name.append(" ");
            Name.append(rs->getString(2));

            cout << Name;    // Print First name + Last name
            cout.width(34);
          
            cout << rs->getString(5) ; // print e-mail

            cout.width(21);
            cout << rs->getString(6);  // print phone

            cout.width(12);
            cout << rs->getString(4);  // print extension
            cout << rs->getString(7) << endl; // reports to


        }

    }
    catch (SQLException& sqlExcp) {
        cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
    }

}

// Add Employee (Option 3)

void insertEmployee(struct Employee* emp) {


    try {
        // Display header
        cout << "-------------- New Employee Information -------------" << endl;

        // Get input from user
        cout << "Employee Number: ";
        cin >> emp->employeeNumber;
        getchar();
        cout << "Last Name: ";
        cin.getline(emp->lastName,30);
        cout << "First Name: ";
        cin.getline(emp->firstName,30);
        cout << "Extension: ";
        cin >> emp->extension;
        cout << "Email: ";
        cin >> emp->email;
        cout << "Office Code: ";
        cin >> emp->officecode;
        cout << "Manager ID: ";
        cin >> emp->reportsTo;
        cout << "Job Title: ";
        cin >> emp->jobTitle;


    } catch (SQLException& sqlExcp) {
        cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();

    }

}

void insertEmployee(Connection* conn, struct Employee emp) {

   
    Statement* stmt = nullptr;
    stmt = conn->createStatement();


    try {

        // Check if the Employee does not exist
        if (!(findEmployee(conn, emp.employeeNumber, &emp))) {

        // Convert the employee number to a string
        string s_empNumber = to_string(emp.employeeNumber);

        // Construct the statement
        string statement = "INSERT INTO employees VALUES("+ s_empNumber +",'" +  emp.lastName + "','" + emp.firstName+ "','" + emp.extension + "','" + emp.email + "','1', 1002,'" + emp.jobTitle + "')";

        // Run the statement
        ResultSet* rs = stmt->executeQuery(statement);
        cout << endl << "The new employee is added successfully." << endl;

        // Commit statement
        conn->commit();
        conn->terminateStatement(stmt);

        }

        // If the employee already exist, show the proper message
        else {
            cout << endl << "An employee with the same employee number exists." << endl;
            
        }

    }
    catch (SQLException& sqlExcp) {
        cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
    }

}

// Update Employee (Option 4)

void updateEmployee(Connection* conn, int employeeNumber) {

    //Used as an empty paramenter
    Employee emp = {0};
    string extension;


    Statement* stmt = nullptr;
    stmt = conn->createStatement();


    try {

        cout << "Extension: ";
        cin >> extension;


        // Construct the statement
      string statement = "UPDATE employees SET EXTENSION = '"+ extension + " ' WHERE EMPLOYEENUMBER =" + to_string(employeeNumber);

       // Run the statement
       ResultSet* rs = stmt->executeQuery(statement);
       cout << endl << "The employee's extension is updated successfully." << endl;

       // Commit statement
       conn->commit();
       conn->terminateStatement(stmt);

          
    }
    catch (SQLException& sqlExcp) {
        cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
    }


}

// Remove Employee (Option 5)

void deleteEmployee(Connection* conn, int employeeNumber) {

    //Used as an empty paramenter
    Employee emp = { 0 };
    string extension;


    Statement* stmt = nullptr;
    stmt = conn->createStatement();

  //  string s_empNumber = to_string(employeeNumber);

    try {

        if (findEmployee(conn, employeeNumber, &emp)) {

            // Construct the statement
            string statement = "DELETE FROM employees WHERE employeenumber = " + to_string(employeeNumber);


            // Run the statement
            ResultSet* rs = stmt->executeQuery(statement);

           // Commit statement
           conn->commit();
           conn->terminateStatement(stmt);

            // Show output to user
            cout << "The employee with ID " << employeeNumber << " is deleted successfully." << endl;


        }
        else
        {
            // If the employee does not exist, show the proper message
            cout << "Employee " << employeeNumber << " does not exist." << endl;
        }


    }
    catch (SQLException& sqlExcp) {
        cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
    }

}


int main(void)

{
    int option = -1;

    /* OCCI Variables */
    Environment* env = nullptr;
    Connection* conn = nullptr;


    /* Used Variables */

    string user = "dbs211_212zb29"; //Enter your sql developer username/password in quotes 
    string pass = "27554301";
    string constr = "myoracle12c.senecacollege.ca:1521/oracle12c";

    env = Environment::createEnvironment(Environment::DEFAULT);
    conn = env->createConnection(user, pass, constr);

    Employee EmpOp3 = { 0 };

    while (option != 0)
    {
        int empNum = 0;
        Employee A,B = {0};

        option = menu();
        switch (option)
        {
        case 1:
            cout << "Enter Employee Number: ";
            cin >> empNum;

            if (findEmployee(conn, empNum, &A)) {


                cout << endl << "-------------- Employee Information -------------" << endl;
            
                cout << "Employee Number: " << A.employeeNumber << endl;
                cout << "Last Name: " << A.lastName << endl;
                cout << "First Name: " << A.firstName << endl;
                cout << "Extension: " << A.extension << endl;
                cout << "Email: " << A.email << endl;
                cout << "Office Code: " << A.officecode << endl;
                cout << "Manager ID: " << A.reportsTo << endl;
                cout << "Job Title: " << A.jobTitle << endl;
            
            }else
            {
                cout << "Employee " << empNum << " does not exist." << endl;
            }


            break;
        case 2:
            displayAllEmployees(conn);
            break;
        case 3:
           // cout << "Add Employee" << endl;
            insertEmployee(&EmpOp3);
            insertEmployee(conn, EmpOp3);
            break;
        case 4:
           // cout << "Update Employee" << endl;
            empNum = 0;
            cout << "Employee Number: ";
            cin >> empNum;

            // Check if the Employee exist
            if (findEmployee(conn, empNum, &B)) {

                cout << "Last Name: " << B.lastName << endl;
                cout << "First Name: " << B.firstName << endl;
                updateEmployee(conn, empNum);

            }  // If the employee does not exist, show the proper message
            else {
                cout << "Employee with the ID: " + to_string(empNum) + " does not exist." << endl;

            }
            break;
        case 5:
           // cout << "Remove Employee" << endl;
            empNum = 0;
            cout << "Employee Number: ";
            cin >> empNum;
            deleteEmployee(conn,empNum);

            break;
        case 0:
            cout << "Exit" << endl;
            break;

        }

    }
    
    // Terminate Connection
    env->terminateConnection(conn);
    Environment::terminateEnvironment(env);

    return 0;

}


