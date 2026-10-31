/*
 * University Course & Exam Scheduler
 * -----------------------------------
 * SIH Smart Education theme project.
 *
 * Generates a clash-free course/exam timetable using:
 *   - a directed graph of prerequisite relationships
 *   - BFS-based Topological Sorting (Kahn's Algorithm)
 *
 * See README.md for build instructions and REPORT.md for the full
 * project write-up (abstract, methodology, complexity analysis, etc).
 */

#include "Course.h"
#include "Graph.h"
#include "Scheduler.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <limits>

// ---------- Small helper functions ----------

// Clears a bad cin state and discards the rest of the line.
// Used after cin >> fails or before mixing >> with getline.
void clearInputLine() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void printMenu() {
    std::cout << "\n============ University Course & Exam Scheduler ============\n";
    std::cout << "1. Add Course\n";
    std::cout << "2. Add Prerequisite\n";
    std::cout << "3. View Course Graph\n";
    std::cout << "4. Generate Timetable\n";
    std::cout << "5. Display Timetable\n";
    std::cout << "6. Detect Cycles\n";
    std::cout << "7. Assign Faculty/Classroom (optional)\n";
    std::cout << "8. Save Timetable to File\n";
    std::cout << "9. Load Courses & Prerequisites from File\n";
    std::cout << "10. Generate Sample Dataset\n";
    std::cout << "11. Display All Courses\n";
    std::cout << "12. Exit\n";
    std::cout << "==============================================================\n";
    std::cout << "Enter choice: ";
}

// Adds a course to both the graph (structure) and the course table (data).
void addCourse(Graph& graph, std::map<std::string, Course>& courses,
               const std::string& name, int credits = 3) {
    if (graph.hasCourse(name)) {
        std::cout << "Course \"" << name << "\" already exists. Skipped.\n";
        return;
    }
    graph.addCourseNode(name);
    courses[name] = Course(name, credits);
    std::cout << "Course \"" << name << "\" added.\n";
}

// Loads courses and prerequisites from a simple text file format:
//   COURSES
//   DSA
//   OOP
//   ...
//   PREREQUISITES
//   DSA DBMS
//   DSA OS
//   ...
// (blank lines are ignored; "PREREQUISITES" section is optional)
bool loadFromFile(const std::string& filename, Graph& graph,
                   std::map<std::string, Course>& courses, Scheduler& scheduler) {
    std::ifstream in(filename);
    if (!in.is_open()) {
        std::cout << "Could not open file \"" << filename << "\".\n";
        return false;
    }

    graph.clear();
    courses.clear();
    scheduler.reset();

    std::string line;
    enum Section { NONE, COURSES, PREREQS };
    Section section = NONE;
    int loadedCourses = 0, loadedEdges = 0, skippedEdges = 0;

    while (std::getline(in, line)) {
        // trim trailing carriage return / whitespace
        while (!line.empty() && (line.back() == '\r' || line.back() == ' '))
            line.pop_back();
        if (line.empty()) continue;

        if (line == "COURSES") { section = COURSES; continue; }
        if (line == "PREREQUISITES") { section = PREREQS; continue; }

        if (section == COURSES) {
            addCourse(graph, courses, line);
            loadedCourses++;
        } else if (section == PREREQS) {
            std::istringstream iss(line);
            std::string from, to;
            if (iss >> from >> to) {
                if (graph.addEdge(from, to)) {
                    loadedEdges++;
                } else {
                    std::cout << "Warning: could not add prerequisite \"" << from
                              << " -> " << to << "\" (course missing or self-loop).\n";
                    skippedEdges++;
                }
            }
        }
    }
    in.close();
    std::cout << "Loaded " << loadedCourses << " courses and " << loadedEdges
              << " prerequisites from \"" << filename << "\"";
    if (skippedEdges > 0) std::cout << " (" << skippedEdges << " prerequisites skipped)";
    std::cout << ".\n";
    return true;
}

// Populates the graph with the sample dataset from the project spec,
// useful for quickly demoing the system.
void generateSampleDataset(Graph& graph, std::map<std::string, Course>& courses,
                           Scheduler& scheduler) {
    graph.clear();
    courses.clear();
    scheduler.reset();

    addCourse(graph, courses, "DSA");
    addCourse(graph, courses, "OOP");
    addCourse(graph, courses, "DBMS");
    addCourse(graph, courses, "OS");
    addCourse(graph, courses, "CN");

    graph.addEdge("DSA", "DBMS");
    graph.addEdge("DSA", "OS");
    graph.addEdge("OOP", "CN");
    graph.addEdge("DBMS", "CN");

    std::cout << "Sample dataset loaded: 5 courses, 4 prerequisite edges.\n";
}

void displayAllCourses(const std::map<std::string, Course>& courses,
                        const Graph& graph) {
    if (courses.empty()) {
        std::cout << "No courses added yet.\n";
        return;
    }
    std::cout << "\n--- All Courses ---\n";
    for (const auto& name : graph.getInsertionOrder()) {
        const Course& c = courses.at(name);
        std::cout << "- " << c.getName() << " (credits: " << c.getCredits() << ")";
        if (c.isScheduled()) std::cout << " [slot " << c.getSlot() << "]";
        std::cout << "\n";
    }
    std::cout << "-------------------\n";
}

// ---------- Main menu loop ----------

int main() {
    Graph graph;
    std::map<std::string, Course> courses;
    Scheduler scheduler(graph, courses);

    std::cout << "Welcome to the University Course & Exam Scheduler.\n";
    std::cout << "(Tip: choose option 10 to load a ready-made sample dataset.)\n";

    int choice = 0;
    bool running = true;

    while (running) {
        printMenu();
        if (!(std::cin >> choice)) {
            std::cout << "Invalid input. Please enter a number.\n";
            clearInputLine();
            continue;
        }
        clearInputLine();

        switch (choice) {
            case 1: {
                std::cout << "Enter course name: ";
                std::string name;
                std::getline(std::cin, name);
                if (name.empty()) { std::cout << "Course name cannot be empty.\n"; break; }

                std::cout << "Enter credits (default 3, press Enter to skip): ";
                std::string creditsInput;
                std::getline(std::cin, creditsInput);
                int credits = 3;
                if (!creditsInput.empty()) {
                    try { credits = std::stoi(creditsInput); }
                    catch (...) { std::cout << "Invalid number, defaulting to 3.\n"; }
                }
                addCourse(graph, courses, name, credits);
                break;
            }
            case 2: {
                std::cout << "Enter prerequisite course (X, where X -> Y): ";
                std::string from;
                std::getline(std::cin, from);
                std::cout << "Enter dependent course (Y, where X -> Y): ";
                std::string to;
                std::getline(std::cin, to);

                if (!graph.hasCourse(from) || !graph.hasCourse(to)) {
                    std::cout << "Error: both courses must exist before adding a "
                                 "prerequisite. Use option 1 first.\n";
                } else if (graph.addEdge(from, to)) {
                    std::cout << "Prerequisite added: " << from << " -> " << to << "\n";
                } else {
                    std::cout << "Could not add edge (possibly a self-loop).\n";
                }
                break;
            }
            case 3:
                graph.displayGraph();
                break;
            case 4: {
                bool ok = scheduler.runKahnTopologicalSort();
                if (!ok) {
                    std::cout << "\nCannot generate timetable: a cyclic dependency "
                                 "was detected among prerequisites.\n";
                    std::cout << "Courses involved in/blocked by the cycle: ";
                    const auto& cyc = scheduler.getCycleNodes();
                    for (size_t i = 0; i < cyc.size(); ++i) {
                        std::cout << cyc[i];
                        if (i + 1 < cyc.size()) std::cout << ", ";
                    }
                    std::cout << "\nFix the prerequisites (remove the cycle) and try again.\n";
                    break;
                }
                scheduler.generateTimetable();

                std::cout << "\nGenerated Course Order:\n";
                const auto& order = scheduler.getTopoOrder();
                for (size_t i = 0; i < order.size(); ++i) {
                    std::cout << order[i];
                    if (i + 1 < order.size()) std::cout << " -> ";
                }
                std::cout << "\n";
                scheduler.displayTimetable();
                break;
            }
            case 5:
                scheduler.displayTimetable();
                break;
            case 6: {
                bool ok = scheduler.runKahnTopologicalSort();
                if (ok) {
                    std::cout << "No cycles detected. Prerequisite graph is valid "
                                 "(a Directed Acyclic Graph).\n";
                } else {
                    std::cout << "Cycle detected! The following courses are "
                                 "involved in / blocked by a cyclic dependency:\n";
                    for (const auto& n : scheduler.getCycleNodes()) {
                        std::cout << "  - " << n << "\n";
                    }
                }
                break;
            }
            case 7: {
                if (!scheduler.isTimetableGenerated()) {
                    std::cout << "Generate the timetable first (option 4).\n";
                    break;
                }
                std::cout << "Enter course name: ";
                std::string course;
                std::getline(std::cin, course);
                std::cout << "Enter faculty name: ";
                std::string faculty;
                std::getline(std::cin, faculty);
                std::cout << "Enter classroom: ";
                std::string room;
                std::getline(std::cin, room);
                scheduler.assignFacultyClassroom(course, faculty, room);
                break;
            }
            case 8: {
                std::cout << "Enter filename to save (e.g. timetable.txt): ";
                std::string filename;
                std::getline(std::cin, filename);
                if (filename.empty()) filename = "timetable.txt";
                scheduler.saveTimetableToFile(filename);
                break;
            }
            case 9: {
                std::cout << "Enter filename to load (e.g. sample_input.txt): ";
                std::string filename;
                std::getline(std::cin, filename);
                if (filename.empty()) filename = "sample_input.txt";
                loadFromFile(filename, graph, courses, scheduler);
                break;
            }
            case 10:
                generateSampleDataset(graph, courses, scheduler);
                break;
            case 11:
                displayAllCourses(courses, graph);
                break;
            case 12:
                std::cout << "Exiting. Goodbye!\n";
                running = false;
                break;
            default:
                std::cout << "Invalid choice. Please select 1-12.\n";
        }
    }

    return 0;
}
