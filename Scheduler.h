#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "Graph.h"
#include "Course.h"
#include <vector>
#include <map>
#include <set>
#include <string>

/*
 * Scheduler
 * ---------
 * Owns the scheduling algorithm. Takes a Graph (prerequisite structure)
 * and a table of Course objects, and produces:
 *   1. A topological order (via Kahn's / BFS algorithm)
 *   2. A clash-free timetable (one course per slot, in topological order)
 *   3. Optional faculty/classroom assignment with overlap checking
 *
 * Design note: Scheduler does not own the Graph or Course data -- it
 * takes references to them. This keeps a clean separation of concerns:
 * Graph = structure, Course = data, Scheduler = algorithm/behavior.
 */
class Scheduler {
private:
    Graph& graph;
    std::map<std::string, Course>& courses;

    std::vector<std::string> topoOrder;   // result of last Kahn's run
    bool cycleDetected;
    std::vector<std::string> cycleNodes;  // courses stuck in a cycle (unprocessed)

    std::map<int, std::string> timetable; // slot number -> course name

    // slot -> set of faculty/classrooms already booked in that slot,
    // used to prevent double-booking.
    std::map<int, std::set<std::string>> facultyBookedPerSlot;
    std::map<int, std::set<std::string>> classroomBookedPerSlot;

public:
    Scheduler(Graph& g, std::map<std::string, Course>& courseTable);

    // Runs Kahn's BFS-based topological sort on `graph`.
    // Returns true if a full ordering was found (no cycle),
    // false if a cycle was detected (topoOrder will be incomplete).
    bool runKahnTopologicalSort();

    const std::vector<std::string>& getTopoOrder() const;
    bool isCycleDetected() const;
    const std::vector<std::string>& getCycleNodes() const;

    // Builds the timetable by assigning each course in topoOrder to the
    // next available slot (slot 1, 2, 3, ...). Requires a successful
    // topological sort first.
    void generateTimetable();

    void displayTimetable() const;

    // Optional enhancement: assign faculty + classroom to a course,
    // rejecting the assignment if it would clash with another course
    // already booked in the same slot.
    bool assignFacultyClassroom(const std::string& courseName,
                                 const std::string& facultyName,
                                 const std::string& classroomName);

    void saveTimetableToFile(const std::string& filename) const;

    bool isTimetableGenerated() const;

    // Clears scheduling results and unassigns slots/faculty/rooms on courses
    void reset();
};

#endif // SCHEDULER_H
