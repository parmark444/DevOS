# DevOS - Development Operating System

A student-friendly Linux distribution I'm building to make systems programming accessible for undergraduate students learning C and operating systems concepts.

## Vision

I want to transform intimidating low-level development tools into approachable, educational experiences. Instead of cryptic error messages and complex command-line syntax, students will receive clear explanations, helpful guidance, and real-time feedback as they learn memory management, debugging, and systems programming fundamentals.

## Core Philosophy

My goal is to make the learning curve less steep by building intelligent wrappers around industry-standard tools (GCC, Valgrind, GDB) that explain what's happening, why errors occur, and how to fix them—without hiding the underlying complexity students need to understand.

## Planned Components

### Custom System Libraries
- Segregated free-list memory allocator with teaching mode
- Thread pool with visual scheduling demonstrations
- Educational implementations of core OS primitives

### Student-Friendly Development Tools
- **devos-compile** - Smart compiler with beginner-friendly error explanations
- **devos-check** - Valgrind wrapper that explains memory errors in plain English
- **devos-debug** - Interactive GDB interface with helpful prompts
- **devos-explain** - Database of common errors with fixes and examples

### Learning Infrastructure
- Interactive tutorials for pointers, memory management, and debugging
- Project templates (linked lists, shells, allocators)
- Built-in performance profiling and visualization

## Target Audience

Computer Science and Computer Engineering undergraduates taking systems programming, operating systems, or C programming courses who need a gentler on-ramp to low-level development without sacrificing technical depth.

## Intended Impact

DevOS will bridge the gap between theoretical OS concepts taught in lectures and practical implementation, helping students build intuition about how operating systems actually work by letting them see, modify, and experiment with real system components.

---

## Development Environment

Currently using a Docker-based isolated development environment for building OS components.

### Setup
```bash
chmod +x build.sh start.sh stop.sh shell.sh clean.sh
./build.sh
./start.sh
```

### Commands
- `./build.sh` - Build the Docker container
- `./start.sh` - Start and enter DevOS environment
- `./stop.sh` - Stop the environment
- `./shell.sh` - Open another terminal in the running container
- `./clean.sh` - Delete container and volume

### Directory Structure
```
/devos/
├── src/
├── include/
├── lib/
├── tests/
└── build/
```

Work persists in Docker volume between sessions.
