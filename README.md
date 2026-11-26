# local-student-portal-project
one of my first attempts at coding,
a very simple console-based crud student portal program fully written in c - not vibe coded at all

## features

- **4 fixed student slots:** store up to 4 student records (id and name) in memory.
- **persistent storage:** automatically loads and saves records to `std.txt` in the same directory as the executable.
- **safe console input:** uses line-buffered reading (`fgets` + `strtol`) to prevent input buffer issues and infinite loops.
- **lowercase cli:** clean, uniform command-line interface.

## requirements

- windows os (uses `<windows.h>` for executable path resolution and `system("cls")` for screen clearing)
- a c compiler such as `gcc` (mingw-w64) or `cl` (msvc)

## build and run

compile using gcc:

```bash
gcc -Wall -Wextra -O2 main.c -o student_portal.exe
