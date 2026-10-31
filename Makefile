CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude
SRC = src/Course.cpp src/Graph.cpp src/Scheduler.cpp src/main.cpp
TARGET = scheduler

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET) timetable.txt

.PHONY: all clean
