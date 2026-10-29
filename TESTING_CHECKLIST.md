# Project Testing Checklist

Use this checklist while testing the program. Record what actually happens; do not mark a test
as passed until you have run it.

- [ ] Compile with `g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o student_manager`
- [ ] Add a valid student (e.g., roll 101, marks 86)
- [ ] Try adding roll 101 again; confirm duplicate is rejected
- [ ] Try marks -1 and 101; confirm validation
- [ ] Display all students
- [ ] Search for an existing roll number
- [ ] Search for a missing roll number
- [ ] Update name/course/marks
- [ ] Delete a record and cancel a deletion once
- [ ] Check result summary
- [ ] Exit and reopen; confirm records persist
- [ ] Try a non-numeric menu choice; confirm program recovers
