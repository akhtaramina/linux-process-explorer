# Linux Process Explorer

A command-line tool written in C++ that reads live process information
directly from the Linux /proc filesystem — the same data source used
by top and htop.

## What It Does

- Lists all running processes on the system
- Shows PID, name, state, and memory usage (RSS) for each process
- Demonstrates fork + exec + pipe by launching ps aux as a child process
  and capturing its output through a pipe
- Handles Ctrl+C gracefully using a SIGINT signal handler

## How It Works

The program walks the /proc directory at runtime. Every numeric
subdirectory inside /proc represents a live process. For each PID,
the program opens /proc/PID/status and parses three fields:

- Name  → process name (truncated to 15 chars by the kernel)
- State → R (running), S (sleeping), Z (zombie), D (waiting)
- VmRSS → physical memory in use, in kilobytes

A separate demo forks a child process, redirects its stdout into a
pipe using dup2, execs ps aux inside the child, and reads the output
back in the parent line by line — demonstrating Unix IPC fundamentals.

## Concepts Demonstrated

- /proc filesystem parsing using C++ file streams
- fork() to create child processes
- exec() to replace a process image with a new program
- Unix pipes for inter-process communication
- dup2() to redirect file descriptors
- SIGINT signal handling with volatile sig_atomic_t
- waitpid() to prevent zombie processes
- Directory traversal with opendir/readdir/closedir

## Build

make

## Run

./process_explorer

## Debug

```
gdb ./process_explorer
(gdb) break main
(gdb) run
(gdb) next
(gdb) print pid
```

## Project Structure

```
linux-process-explorer/
├── src/
│   ├── main.cpp            → signal handling, fork/exec/pipe, display
│   ├── process.h           → Process struct definition
│   ├── process.cpp         → /proc/PID/status parser
│   └── process_manager.cpp → /proc directory walker
├── Makefile
└── README.md
```


## Environment

- Ubuntu 24.04 on VirtualBox
- g++ 13.3.0
- Debugged with gdb
