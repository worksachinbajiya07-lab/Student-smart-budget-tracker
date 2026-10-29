# Student Management System (C++)

## Project overview
A beginner-friendly, console-based C++ application for adding, displaying, searching,
updating, and deleting student records. It also calculates grades and displays a result
summary. Records are saved in `students.txt`.

## Requirements
- C++ compiler (g++ recommended)
- Terminal / Command Prompt, or an IDE such as Code::Blocks or VS Code

## Compile and run
### macOS / Linux
```bash
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o student_manager
./student_manager
```

### Windows (MinGW)
```bat
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o student_manager.exe
student_manager.exe
```

The `students.txt` file is created in the current working directory when records are saved.
Keep it alongside the program's working directory so records can be loaded next time.

## Menu
1. Add student
2. Display all students
3. Search student
4. Update student
5. Delete student
6. Result summary
0. Save and exit

## Grade criteria used
- 90–100: A+
- 80–89.99: A
- 70–79.99: B
- 60–69.99: C
- 50–59.99: D
- Below 50: F

The summary counts marks of 40 and above as passing. These are sample rules for this project,
not an official college grading policy.

## Notes and limitations
- This is an educational, local console project, not a secure student database.
- Do not use real sensitive student data for demonstrations.
- Names and courses cannot contain the `|` separator because the program uses it in its
  simple text-file format.
