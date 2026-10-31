# summer-intern-project
University Course & Exam Scheduler
A menu-driven C++ console application that creates a
prerequisite-respecting course timetable using Graph Theory and
Kahn's BFS-based Topological Sorting Algorithm.

Project theme: Smart Education (SIH-inspired)
Language standard: C++17 in the included CMake configuration
Author: Piyush Kumar
Registration No.: 12417684
Department: Computer Science and Engineering, Lovely Professional
University
Overview
University course planning involves prerequisite relationships between
subjects. For example, a student may need to complete DSA before taking
DBMS. Scheduling without considering these dependencies can produce an
invalid course order.

This project models courses as vertices in a directed graph and
prerequisite relationships as directed edges. It uses Kahn's algorithm
to produce a valid topological order and detects cycles that make a
valid order impossible.

The current implementation assigns one course per timetable slot, in
topological order.
Features
- Add and manage course records
- Add prerequisite relationships between courses
- Display the course prerequisite graph
- Generate a course order using Kahn's algorithm
- Detect prerequisite cycles and report unprocessed courses
- Generate and display a prerequisite-respecting timetable
- Optionally assign faculty and classrooms with same-slot
  double-booking checks
- Load course and prerequisite data from a text file
- Generate a built-in sample dataset
- Save the generated timetable to a text file
Technologies
- C++17
- Graphs and adjacency lists
- Breadth-First Search (BFS)
- Kahn's topological sorting algorithm
- std::map, std::vector, std::queue, and std::set
- CMake, Make, or the included Windows build script
- Visual Studio Code (recommended development environment)
