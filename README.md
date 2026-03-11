# CCompiler

A from-scratch C compiler targeting the x86-64 architecture, written entirely in C++17. 

This project is an educational deep-dive into compiler theory, systems programming, and computer architecture. It intentionally avoids heavy backend frameworks like LLVM to build a foundational understanding of Lexical Analysis, Abstract Syntax Trees (AST), Register Allocation, and System V AMD64 ABI compliance.

## Architecture

The compiler is structured into a classic frontend and backend pipeline:

* **Lexer (`src/lexer/`)**: A custom-built lexical analyzer that consumes raw C source code and produces a structured stream of tokens.
* **Parser (`src/parser/`)**: A recursive descent parser that validates syntax and constructs an Abstract Syntax Tree (AST).
* **Code Generator (`src/codegen/`)**: The backend that traverses the AST and emits valid, executable x86-64 assembly code.

## Roadmap: Feature-Driven Development

The compiler is being built incrementally. Each phase represents a fully functional compiler that supports a growing subset of the C language.

- [x] **Phase 1: The Foundation** - Compiling `int main() { return <const>; }` to working assembly. *(In Progress)*
- [ ] **Phase 2: Operations** - Unary (`-`, `~`, `!`) and binary (`+`, `-`, `*`, `/`) arithmetic with precedence.
- [ ] **Phase 3: State** - Local variables, assignments, and stack frame management.
- [ ] **Phase 4: Control Flow** - Conditional branching (`if`/`else`) and loops (`while`, `for`), including short-circuiting logic.
- [ ] **Phase 5: Functions** - Function calls, definitions, and strict adherence to the System V AMD64 Calling Convention.
- [ ] **Phase 6: Memory** - Pointers, address-of/dereference operators, and arrays.

## Prerequisites

To build and run this compiler, you will need:
* A modern C++17 compiler (MSVC, GCC, or Clang)
* CMake (Version 3.10 or higher)
* `gcc` or `nasm` (to assemble and link the output `.s` files into executables)

## Building the Project

This project uses CMake for its build system. From the root directory:

```bash
# 1. Create a build directory
mkdir build
cd build

# 2. Generate the build files
cmake ..

# 3. Compile the project
cmake --build .

```

## Usage

Run the compiler by passing a `.c` source file as an argument. The compiler will generate an `assembly.s` file containing the x86-64 instructions.

```bash
./ccompiler test_program.c

```

To assemble, link, and run the resulting output on a Linux/macOS system:

```bash
gcc assembly.s -o program
./program
echo $? # View the return code

```