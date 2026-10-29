#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>

using namespace std;

struct Student {
    int rollNumber;
    string name;
    string course;
    double marks;
};

const string DATA_FILE = "students.txt";

vector<Student> students;

// Read a complete line and safely handle spaces in names/courses.
string readLine(const string& prompt) {
    string value;
    cout << prompt;
    getline(cin, value);
    return value;
}

int readInt(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        cout << "Invalid input. Please enter a whole number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

double readMarks(const string& prompt) {
    double value;
    while (true) {
        cout << prompt;
        if (cin >> value && value >= 0 && value <= 100) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        cout << "Marks must be a number from 0 to 100.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

int findStudentIndex(int rollNumber) {
    for (size_t i = 0; i < students.size(); ++i) {
        if (students[i].rollNumber == rollNumber) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

string gradeFor(double marks) {
    if (marks >= 90) return "A+";
    if (marks >= 80) return "A";
    if (marks >= 70) return "B";
    if (marks >= 60) return "C";
    if (marks >= 50) return "D";
    return "F";
}

void loadStudents() {
    ifstream file(DATA_FILE);
    if (!file) return;

    students.clear();
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string rollText, name, course, marksText;

        if (getline(ss, rollText, '|') &&
            getline(ss, name, '|') &&
            getline(ss, course, '|') &&
            getline(ss, marksText)) {
            try {
                Student s;
                s.rollNumber = stoi(rollText);
                s.name = name;
                s.course = course;
                s.marks = stod(marksText);
                students.push_back(s);
            } catch (...) {
                // Ignore malformed records rather than stopping the program.
            }
        }
    }
}

bool saveStudents() {
    ofstream file(DATA_FILE);
    if (!file) {
        cout << "Error: unable to save records to " << DATA_FILE << ".\n";
        return false;
    }

    for (const Student& s : students) {
        // The simple text-file format uses | as a separator.
        file << s.rollNumber << '|'
             << s.name << '|'
             << s.course << '|'
             << s.marks << '\n';
    }
    return true;
}

void addStudent() {
    cout << "\n--- Add Student ---\n";
    int roll = readInt("Enter roll number: ");

    if (roll <= 0) {
        cout << "Roll number must be positive.\n";
        return;
    }
    if (findStudentIndex(roll) != -1) {
        cout << "That roll number already exists.\n";
        return;
    }

    string name = readLine("Enter student name: ");
    string course = readLine("Enter course/branch: ");
    if (name.empty() || course.empty() ||
        name.find('|') != string::npos || course.find('|') != string::npos) {
        cout << "Name/course cannot be empty or contain the | character.\n";
        return;
    }

    double marks = readMarks("Enter marks (0-100): ");
    students.push_back({roll, name, course, marks});

    if (saveStudents()) cout << "Student added and saved successfully.\n";
}

void displayStudents() {
    cout << "\n--- Student Records ---\n";
    if (students.empty()) {
        cout << "No student records found.\n";
        return;
    }

    cout << left << setw(12) << "Roll No."
         << setw(26) << "Name"
         << setw(22) << "Course"
         << right << setw(9) << "Marks"
         << setw(9) << "Grade" << '\n';
    cout << string(78, '-') << '\n';

    for (const Student& s : students) {
        cout << left << setw(12) << s.rollNumber
             << setw(26) << s.name.substr(0, 24)
             << setw(22) << s.course.substr(0, 20)
             << right << setw(9) << fixed << setprecision(2) << s.marks
             << setw(9) << gradeFor(s.marks) << '\n';
    }
}

void searchStudent() {
    cout << "\n--- Search Student ---\n";
    int roll = readInt("Enter roll number to search: ");
    int index = findStudentIndex(roll);

    if (index == -1) {
        cout << "Student not found.\n";
        return;
    }

    const Student& s = students[index];
    cout << "Roll number : " << s.rollNumber << '\n'
         << "Name        : " << s.name << '\n'
         << "Course      : " << s.course << '\n'
         << "Marks       : " << fixed << setprecision(2) << s.marks << '\n'
         << "Grade       : " << gradeFor(s.marks) << '\n';
}

void updateStudent() {
    cout << "\n--- Update Student ---\n";
    int roll = readInt("Enter roll number to update: ");
    int index = findStudentIndex(roll);

    if (index == -1) {
        cout << "Student not found.\n";
        return;
    }

    Student& s = students[index];
    cout << "Press Enter to keep the current name/course.\n";
    string name = readLine("New name [" + s.name + "]: ");
    string course = readLine("New course [" + s.course + "]: ");

    if (!name.empty() && name.find('|') == string::npos) s.name = name;
    if (!course.empty() && course.find('|') == string::npos) s.course = course;

    cout << "Current marks: " << s.marks << '\n';
    string changeMarks = readLine("Update marks? (y/n): ");
    if (changeMarks == "y" || changeMarks == "Y") {
        s.marks = readMarks("Enter new marks (0-100): ");
    }

    if (saveStudents()) cout << "Student record updated successfully.\n";
}

void deleteStudent() {
    cout << "\n--- Delete Student ---\n";
    int roll = readInt("Enter roll number to delete: ");
    int index = findStudentIndex(roll);

    if (index == -1) {
        cout << "Student not found.\n";
        return;
    }

    string confirm = readLine("Delete this record? (y/n): ");
    if (confirm == "y" || confirm == "Y") {
        students.erase(students.begin() + index);
        if (saveStudents()) cout << "Student record deleted successfully.\n";
    } else {
        cout << "Delete cancelled.\n";
    }
}

void showSummary() {
    cout << "\n--- Result Summary ---\n";
    if (students.empty()) {
        cout << "No student records available.\n";
        return;
    }

    double total = 0;
    int passed = 0;
    for (const Student& s : students) {
        total += s.marks;
        if (s.marks >= 40) ++passed;
    }

    cout << "Total students : " << students.size() << '\n'
         << "Average marks  : " << fixed << setprecision(2)
         << total / students.size() << '\n'
         << "Passed (40+)   : " << passed << '\n'
         << "Below 40       : " << students.size() - passed << '\n';
}

void showMenu() {
    cout << "\n========================================\n"
         << "       STUDENT MANAGEMENT SYSTEM\n"
         << "========================================\n"
         << "1. Add student\n"
         << "2. Display all students\n"
         << "3. Search student\n"
         << "4. Update student\n"
         << "5. Delete student\n"
         << "6. Result summary\n"
         << "0. Save and exit\n"
         << "----------------------------------------\n";
}

int main() {
    loadStudents();
    cout << "Welcome! Loaded " << students.size() << " saved record(s).\n";

    while (true) {
        showMenu();
        int choice = readInt("Enter your choice: ");

        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6: showSummary(); break;
            case 0:
                saveStudents();
                cout << "Records saved. Thank you for using the system!\n";
                return 0;
            default:
                cout << "Invalid choice. Please select a menu option.\n";
        }
    }
}
