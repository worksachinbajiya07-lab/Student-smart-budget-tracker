# SUMMER INTERNSHIP PROJECT REPORT
## STUDENT MANAGEMENT SYSTEM USING C++

**Submitted by:** Sachin Bajiya  
**College:** Jaipur Engineering College and Research Centre, Jaipur  
**Branch:** Information Technology  
**Year/Semester:** Second Year / Third Semester  
**Internship organization:** InternPe  
**Project type:** Console-based C++ application

> Before submission, add your official internship dates, faculty/mentor details, college logo,
> certificate, acknowledgement, and any required institute formatting. Only claim activities
> and internship work that you actually completed or can verify.

---

## 1. Abstract
The Student Management System is a console-based application developed in C++. It helps a user
maintain basic student records using a menu-driven interface. The application supports adding,
displaying, searching, updating, and deleting student information. It also calculates a grade
from marks and displays a summary of student results. File handling is used to preserve records
between program runs. The project demonstrates fundamental programming concepts such as
structures, functions, loops, conditional statements, vectors, searching, and file input/output.

## 2. Introduction
Student information is often maintained in registers or separate files. A small computer-based
system can make basic record operations more organized. This project provides a simple educational
example of how C++ can be used to manage records in a console environment. It is designed for
learning and demonstration rather than as a production-ready institutional database.

## 3. Problem Statement
Manual handling of student records can make it time-consuming to locate, correct, and summarize
information. The objective is to build a simple program that provides common record-management
operations through one menu and saves the records locally.

## 4. Objectives
- Build a menu-driven application using C++.
- Store student roll number, name, course/branch, and marks.
- Add, display, search, update, and delete student records.
- Calculate grades and a basic result summary.
- Use file handling to save and reload records.
- Practice modular programming, input validation, and testing.

## 5. Scope
The application is suitable for a classroom demonstration or a small sample dataset. It runs
locally in a terminal and stores data in a text file. It does not include a graphical interface,
multi-user access, database server, login system, or institutional integration.

## 6. Tools and Technologies
- Language: C++
- Standard library: iostream, fstream, sstream, vector, string, iomanip, limits
- Compiler: g++ or another C++17-compatible compiler
- Interface: command-line / console
- Storage: local text file (`students.txt`)

## 7. System Requirements
**Hardware:** A basic laptop or desktop capable of running a C++ compiler.  
**Software:** Windows, macOS, or Linux; a C++ compiler; and a text editor or IDE.

## 8. Functional Requirements
1. Add a student with a unique positive roll number.
2. Display all saved student records.
3. Search a record by roll number.
4. Update a student's name, course, or marks.
5. Delete a record after confirmation.
6. Show total students, average marks, and pass/below-40 counts.
7. Load saved records at startup and save after changes.

## 9. Design and Working
The program starts by loading records from `students.txt`. It then displays a menu and reads the
user's choice. A switch statement calls the relevant function. The student records are held in a
vector while the application runs. Search operations compare roll numbers. Add, update, and delete
operations save the updated vector to the text file. Selecting exit saves the data and closes the
program.

### Main modules
- `loadStudents()`: reads records from the local text file.
- `saveStudents()`: writes the current records to the file.
- `addStudent()`: validates and adds a record.
- `displayStudents()`: displays all records.
- `searchStudent()`: finds a record by roll number.
- `updateStudent()`: modifies an existing record.
- `deleteStudent()`: removes a record after confirmation.
- `showSummary()`: calculates totals, average, and result counts.
- `gradeFor()`: maps marks to a sample grade.

### Algorithm
1. Start the program.
2. Load previously saved student records.
3. Display the main menu.
4. Read the user's choice.
5. Execute the selected operation.
6. Save changes when applicable.
7. Repeat until the user chooses exit.
8. Save records and terminate.

## 10. Data Structure and File Format
A `Student` structure contains roll number, name, course, and marks. A vector stores the current
set of students. Each line in `students.txt` follows this pattern:

`rollNumber|name|course|marks`

The pipe character is used as a separator and is not permitted in names or course fields.

## 11. Sample Output
```text
Welcome! Loaded 0 saved record(s).

========================================
       STUDENT MANAGEMENT SYSTEM
========================================
1. Add student
2. Display all students
3. Search student
4. Update student
5. Delete student
6. Result summary
0. Save and exit
----------------------------------------
Enter your choice: 1

--- Add Student ---
Enter roll number: 101
Enter student name: Aditi Sharma
Enter course/branch: Information Technology
Enter marks (0-100): 86
Student added and saved successfully.
```

## 12. Testing Plan
Run each test and record the actual result in the final report.

| Test | Input/action | Expected result |
|---|---|---|
| Add valid record | Unique positive roll, valid name/course, marks 0–100 | Record saved |
| Duplicate roll | Add an existing roll number | Duplicate rejected |
| Invalid marks | Enter marks outside 0–100 | Input rejected and requested again |
| Display records | Choose option 2 | Records shown in a table |
| Search existing | Enter a saved roll number | Correct record displayed |
| Search missing | Enter an unknown roll number | “Student not found” shown |
| Update record | Change an existing student's details | Updated values saved |
| Delete record | Confirm deletion with `y` | Record removed |
| Persistence | Exit and reopen program | Saved records loaded |
| Empty data | Display or summary with no records | Informative empty-state message |

## 13. Results and Discussion
The implementation provides a simple menu-driven workflow for managing sample student records.
The text-file approach demonstrates persistence without requiring an external database. The
program validates roll number uniqueness and marks range, and separates major operations into
functions. Complete the testing table with results observed on your own machine before submission.

## 14. Limitations
- Records are stored in a plain text file and are not encrypted.
- The application is designed for local, single-user use.
- The simple delimiter format does not support the `|` character in names or courses.
- Sample grading and passing criteria may not match official academic rules.
- No authentication, database, or graphical user interface is included.

## 15. Future Enhancements
- Add a graphical interface using a suitable C++ GUI framework.
- Replace text-file storage with SQLite or another database.
- Add sorting and filtering by name, marks, or course.
- Add attendance and subject-wise marks.
- Add user authentication and role-based access.
- Export reports to CSV.

## 16. Conclusion
The Student Management System demonstrates how core C++ features can be combined to build a useful
console application. It provides experience with structured data, functions, input validation,
searching, vectors, and file handling. The project can be expanded as programming skills improve.

## 17. References
- C++ reference: https://en.cppreference.com/
- ISO C++: https://isocpp.org/
- Compiler documentation for the compiler used during development.

---

## Suggested presentation outline
1. Title and student details
2. Project introduction
3. Problem statement and objectives
4. Tools and technologies
5. System features
6. System workflow / algorithm
7. C++ concepts used
8. Sample output and demonstration
9. Testing and limitations
10. Future scope and conclusion
