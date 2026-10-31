#ifndef COURSE_H
#define COURSE_H

#include <string>

/*
 * Course
 * ------
 * Represents a single course/subject offered by the university.
 * Holds identity data (name, credits) plus scheduling data that
 * gets filled in later by the Scheduler (slot, faculty, classroom).
 *
 * Kept deliberately simple (a data holder with accessors) so that
 * Graph and Scheduler can stay focused on algorithmic logic.
 */
class Course {
private:
    std::string name;
    int credits;
    std::string faculty;      // assigned faculty member (empty if unassigned)
    std::string classroom;    // assigned classroom (empty if unassigned)
    int slot;                 // assigned timetable slot, -1 if unassigned

public:
    // Default credits = 3 if not specified by the user
    explicit Course(const std::string& courseName = "", int courseCredits = 3);

    // Accessors
    std::string getName() const;
    int getCredits() const;
    std::string getFaculty() const;
    std::string getClassroom() const;
    int getSlot() const;

    // Mutators
    void setCredits(int c);
    void setFaculty(const std::string& f);
    void setClassroom(const std::string& c);
    void setSlot(int s);

    // Convenience: has this course been placed in the timetable yet?
    bool isScheduled() const;
};

#endif // COURSE_H
