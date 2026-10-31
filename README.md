# University Course & Exam Scheduler

A C++ console application that generates a clash-free course/exam timetable
using Graph Theory and BFS-based Topological Sorting (Kahn's Algorithm).

Built for the SIH Smart Education theme.

## 1. Problem It Solves

Course timetables aren't arbitrary — some subjects must come before others
(e.g. DSA before DBMS). If you schedule courses without respecting these
prerequisite chains, or if the prerequisites themselves contain a
contradiction (a cycle), the timetable is invalid. This tool:

- Models courses as nodes and prerequisites as directed edges.
- Runs Kahn's Algorithm to get a valid processing order.
- Detects prerequisite cycles and reports them instead of producing a
  broken timetable.
- Assigns each course to a slot, in an order that never violates a
  prerequisite.
- Optionally checks faculty/classroom assignments for double-booking.

## 2. Project Structure

```
university_scheduler/
├── include/
│   ├── Course.h        # Course data (name, credits, faculty, room, slot)
│   ├── Graph.h          # Directed graph (adjacency list + indegree)
│   └── Scheduler.h      # Kahn's algorithm + timetable generation
├── src/
│   ├── Course.cpp
│   ├── Graph.cpp
│   ├── Scheduler.cpp
│   └── main.cpp          # Menu-driven console UI
├── sample_input.txt       # Ready-to-load sample dataset (matches spec example)
├── sample_cycle_input.txt # Cycle-detection demo dataset
├── sample_output.txt      # Expected output for the spec example
├── build.bat              # Windows build helper
├── CMakeLists.txt         # Cross-platform CMake build
├── Makefile
├── README.md
└── REPORT.md              # Full mini-project report
```

### Why three classes

| Class       | Responsibility                                             |
|-------------|--------------------------------------------------------------|
| `Course`    | Pure data holder — name, credits, assigned faculty/room/slot |
| `Graph`     | Structure only — adjacency list, indegree tracking            |
| `Scheduler` | Behavior — runs Kahn's algorithm, builds the timetable, checks clashes |

This separation means the algorithm (`Scheduler`) doesn't need to know how
the graph is stored internally, and the graph doesn't need to know
anything about timetables. Each class can be tested or reused
independently.

## 3. Algorithm — Kahn's BFS Topological Sort

**Graph model:** each course is a node; edge `A -> B` means *A is a prerequisite of B* (A must be scheduled before B).

**Steps (`Scheduler::runKahnTopologicalSort`):**

1. Copy indegree counts for every course (maintained in `Graph` as edges are added).
2. Enqueue all courses with indegree `0` (no unmet prerequisites), using insertion order as a tie-break.
3. While the queue is not empty:
   - Dequeue course `u`, append to the result order.
   - For each dependent course `v` of `u`, decrement `v`'s indegree; if it reaches `0`, enqueue `v`.
4. If the result contains all courses → valid DAG, scheduling can proceed.
5. If any course remains with indegree `> 0` → **cycle detected**; those courses are reported.

**Timetable:** courses are placed one per slot in topological order, guaranteeing no prerequisite violation.

| Metric | Complexity |
|--------|------------|
| Kahn's Algorithm | **O(V + E)** time, **O(V + E)** space |
| Timetable generation | O(V) time |

See `REPORT.md` for the full complexity table and methodology.

## 4. Building

Requires a C++17 compiler (g++, clang++, or MSVC `cl`).

### Run with F5 in VS Code (recommended)

1. Extract/open the project folder in VS Code.
2. Install the **C/C++** extension (Microsoft).
3. Install **MinGW-w64** (`g++` and `gdb`) and add them to your PATH.
4. Press **F5** (or **Run → Start Debugging**) — this builds and runs `scheduler.exe` in the integrated terminal.
5. Use **Ctrl+Shift+B** to build only, or **Terminal → Run Task → Run Scheduler** to build and run without the debugger.

VS Code config files are in `.vscode/` (`tasks.json`, `launch.json`, `c_cpp_properties.json`).

**Linux / macOS (Make):**

```bash
make            # builds ./scheduler
./scheduler
```

**Windows (batch script):**

```bat
build.bat       # auto-detects g++ or cl, builds scheduler.exe
scheduler.exe
```

**CMake (all platforms):**

```bash
cmake -S . -B build
cmake --build build
# executable: build/scheduler (or build\Debug\scheduler.exe on MSVC)
```

**Manual compile:**

```bash
g++ -std=c++17 -Wall -Wextra -Iinclude src/Course.cpp src/Graph.cpp src/Scheduler.cpp src/main.cpp -o scheduler
```

## 5. Using the Menu

```
1. Add Course
2. Add Prerequisite
3. View Course Graph
4. Generate Timetable
5. Display Timetable
6. Detect Cycles
7. Assign Faculty/Classroom (optional)
8. Save Timetable to File
9. Load Courses & Prerequisites from File
10. Generate Sample Dataset
11. Display All Courses
12. Exit
```

Fastest way to see it work: run `./scheduler`, choose **10** (loads the
sample dataset), then **4** (generates the timetable).

### Loading your own data (option 9)

Use a text file in this format:

```
COURSES
DSA
OOP
DBMS
OS
CN

PREREQUISITES
DSA DBMS
DSA OS
OOP CN
DBMS CN
```

Each line under `PREREQUISITES` is `FROM TO`, meaning `FROM` must be
scheduled before `TO`. A ready-made copy of this exact file is included as
`sample_input.txt`.

## 6. Sample Run

Input (via option 10, the built-in sample dataset):

```
Courses: DSA, OOP, DBMS, OS, CN
Prerequisites: DSA->DBMS, DSA->OS, OOP->CN, DBMS->CN
```

Output (option 4):

```
Generated Course Order:
DSA -> OOP -> DBMS -> OS -> CN

--- Generated Timetable ---
Slot 1: DSA
Slot 2: OOP
Slot 3: DBMS
Slot 4: OS
Slot 5: CN
----------------------------
```

This matches the expected output in the project specification exactly —
verified by an automated test run during development.

## 7. Known Limitation (stated plainly, not glossed over)

This implementation assigns **one course per slot**, in topological order.
That is correct and clash-free by construction, but it also means the
faculty/classroom overlap check (option 7) can never actually trigger a
clash in the current model — no two courses ever share a slot, so there's
nothing to overlap.

The overlap-detection code is real and functional (it works off slot
numbers, not course identity), but it's currently a no-op in practice. To
make it meaningful, the scheduling strategy would need to change from
"one course per slot" to "pack all mutually-independent courses from the
same BFS layer into one slot" (i.e., parallel exam sessions). That's a
natural extension — see REPORT.md, Section 7 (Limitations & Future Work)
— but it was **not** implemented here because it changes the output format
away from the one specified in the project brief. Flagging this rather
than quietly shipping dead code.

## 8. Files Generated at Runtime

- `timetable.txt` (or whatever name you choose in option 8) — plain-text
  timetable, created only if you use the save option. Not committed to
  the repo.
