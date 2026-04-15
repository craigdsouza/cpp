#include <iostream>
#include <iomanip>
#include <string>
#include <ctime>
#include <cstdlib>
// Add a component that uses mathematical functions from the <cmath> header
// Create a component that displays a simple animation using loops and timing
// Implement a component that formats and displays a simple CSV dataset


using namespace std;

int main(){
    // int num = rand();
    // cout << "random number: " << num << " time: "<< time(NULL) << endl;

    // Program title and introduction
    cout << "================================================" << endl;
    cout << "           MULTI-COMPONENT PROGRAM              " << endl;
    cout << "================================================" << endl;
    cout << "This program demonstrates using multiple headers" << endl;
    cout << "and formatting techniques in C++." << endl << endl;
    
    // Component 1: Basic information display
    cout << "------------- COMPONENT 1: BASIC INFO -------------" << endl;

    // Create variables to store information
    /*
    A string is a sequence of characters 
    A variable is a storage location that holds a value
    */
    string name = "C++ Programming";
    string version = "C++17";
    string creator = "Bjarne Stroustrup";
    int yearCreated = 1985;
    string fact1 = "Bell";
    bool fact2 = true;
    int fact3 = 500;

    // Display the information
    cout << "Language: " << name << endl;
    cout << "Version: " << version << endl;
    cout << "Creator: " << creator << endl;
    cout << "Year Created: " << yearCreated << endl << endl;
    cout << "Which lab was it created at: " << fact1 << endl;
    cout << "Is it a static typed language: " << (fact2? "Yes":"No") << endl;
    cout << "Approximately how many C++ developers existed in 1985? " << fact3 << endl;

    // Component 2: Formatted numeric output
    cout << "---------- COMPONENT 2: FORMATTED NUMBERS ---------" << endl;

    // Create numeric variables
    double pi = 3.14159265358979323846;
    double e = 2.71828182845904523536;
    double goldenRatio = 1.61803398874989484820;

    // Display with different precision
    cout << "Default display:" << endl;
    cout << "Pi = " << pi << endl;
    cout << "e = " << e << endl;
    cout << "Golden Ratio = " << goldenRatio << endl << endl;

    cout << "With 2 decimal places:" << endl;
    cout << fixed << setprecision(2);
    cout << "Pi = " << pi << endl;
    cout << "e = " << e << endl;
    cout << "Golden Ratio = " << goldenRatio << endl << endl;

    cout << "With 6 decimal places:" << endl;
    cout << fixed << setprecision(6);
    cout << "Pi = " << pi << endl;
    cout << "e = " << e << endl;
    cout << "Golden Ratio = " << goldenRatio << endl << endl;

    // Reset formatting
    cout.unsetf(ios::fixed);
    cout << setprecision(6);

    // Component 3: Table display
    cout << "------------- COMPONENT 3: TABLE DATA ------------" << endl;

    // Set up table header
    // setw sets the field width for the next output 
    cout << left << setw(15) << "Data Type" << setw(20) << "Size (bytes)" << setw(20) << "Value Range" << endl;
    cout << string(55, '-') << endl;

    // Table rows
    cout << left << setw(15) << "int" << left << setw(20) << sizeof(int) << left << setw(20) << "  -2^31 to 2^31-1" << endl;
    cout << left << setw(15) << "double" << left << setw(20) << sizeof(double) << left << setw(20) << "  ±1.7e^308" << endl;
    cout << left << setw(15) << "char" << left << setw(20) << sizeof(char) << left << setw(20) << "  -128 to 127" << endl;
    cout << left << setw(15) << "bool" << left << setw(20) << sizeof(bool) << left << setw(20) << "  true or false" << endl;
    cout << left << setw(15) << "float" << left << setw(20) << sizeof(float) << left << setw(20) << "3.4E +/- 38 (seven digits) " << endl;
    cout << left << setw(15) << "unsigned int" << left << setw(20) << sizeof(unsigned) << left << setw(20) << "0 to 4,294,967,295 " << endl;
    

    cout << "\n------------- COMPONENT 4: PROGRAM INFO ------------" << endl;

    Get current date/time info (simulated)
    string currentDate = "2023-08-15";
    string userName = "C++ Learner";
    int linesOfCode = 75;

    // Format and display program information
    cout << "Program: Multi-Component Example" << endl;
    cout << "Author: " << userName << endl;
    cout << "Date: " << currentDate << endl;
    cout << "Code Statistics:" << endl;
    cout << " - Lines of code: " << linesOfCode << endl;
    cout << " - Header files: 3" << endl;
    cout << " - Components: 4" << endl;

    // Display a progress bar (simulated)
    cout << "Completion: [";
    int progress = 80; // 80% complete
    // A loop allows something to happen over and over again 
    for (int i = 0; i < 20; i++) {
        if (i < progress/5) cout << "=";
        else cout << " ";
    }
    cout << "] " << progress << "%" << endl;
    // Program end
    cout << "\nProgram execution completed." << endl;
    
    return 0;
}
