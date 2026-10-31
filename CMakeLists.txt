cmake_minimum_required(VERSION 3.14)
project(UniversityScheduler VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(scheduler
    src/main.cpp
    src/Course.cpp
    src/Graph.cpp
    src/Scheduler.cpp
)

target_include_directories(scheduler PRIVATE include)

if(MSVC)
    target_compile_options(scheduler PRIVATE /W4)
else()
    target_compile_options(scheduler PRIVATE -Wall -Wextra)
endif()
