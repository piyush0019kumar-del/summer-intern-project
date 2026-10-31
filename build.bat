@echo off
REM Build script for Windows (MinGW g++ or MSVC cl)

set SRC=src\Course.cpp src\Graph.cpp src\Scheduler.cpp src\main.cpp
set INC=-Iinclude
set OUT=scheduler.exe

where g++ >nul 2>&1
if %ERRORLEVEL%==0 (
    echo Building with g++ ...
    g++ -std=c++17 -Wall -Wextra %INC% %SRC% -o %OUT%
    if %ERRORLEVEL%==0 echo Build successful: %OUT%
    exit /b %ERRORLEVEL%
)

where cl >nul 2>&1
if %ERRORLEVEL%==0 (
    echo Building with MSVC cl ...
    cl /EHsc /std:c++17 /W4 %INC% %SRC% /Fe:%OUT%
    if %ERRORLEVEL%==0 echo Build successful: %OUT%
    exit /b %ERRORLEVEL%
)

echo ERROR: No C++ compiler found. Install MinGW-w64 ^(g++^) or Visual Studio Build Tools ^(cl^).
exit /b 1
