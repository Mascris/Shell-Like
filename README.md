# Shell-Like

A small shell implementation written in C, built as a learning project.

## About

I'm building this project to improve my understanding of C, Linux, and how command-line shells work internally.

The goal is to implement shell functionality from scratch while learning about processes, system calls, command execution, and input/output redirection.

## Features

- Built-in commands:
  - `cd`
  - `pwd`
  - `echo`
  - `exit`
- Execute external commands using `PATH`.
- Input redirection (`<`).
- Output redirection (`>` and `1>`).
- Error redirection (`2>`).
- Process creation using `fork()` and `execvp()`.
- Change and retrieve the current working directory.

## Built With

- C
- GCC
- Linux
- Make
- Git

## Build and Run

Compile the project:

```bash
make
```

Run the shell:

```bash
./shell
```

Remove generated files:

```bash
make fclean
```

## Project Structure

```text
Shell-Like/
├── main.c
├── process.c
├── builtins.c
├── shell.h
├── Makefile
├── README.md
└── .gitignore
```

## Goals

- Improve my C programming skills.
- Understand how shells parse and execute commands.
- Learn more about Linux and Unix-like systems.
- Gain practical experience with systems programming.
- Understand process management and Linux system calls.

## Progress

This project is a work in progress. Features and improvements will be added as I learn more about C and Linux internals.

## License

No license. This is a personal learning project.
