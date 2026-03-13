# CCompiler

A high-performance C compiler targeting the x86-64 architecture, implemented in C++17.

This project is a rigorous implementation of compiler theory, focusing on the construction of a complete translation pipeline from source to executable machine code. The architecture is designed for strict adherence to the ISO C11 standard while maintaining a custom backend to demonstrate deep integration with the System V AMD64 ABI and manual register allocation strategies.

## Compiler Architecture

The translation pipeline follows a modular design, separating concerns between lexical analysis, syntax representation, and target code generation.

* **Lexical Analyzer (Lexer)**: A hand-written deterministic finite automaton (DFA) that performs Phase 3 translation. It handles complex C11 tokens, including hexadecimal floating-point literals and Universal Character Names (UCNs).
* **Syntax Analyzer (Parser)**: A recursive-descent parser responsible for enforcing C11 grammar constraints and generating a high-fidelity Abstract Syntax Tree (AST).
* **Code Generator (Backend)**: A target-specific engine that performs AST lowering, stack frame management, and emits x86-64 assembly compliant with the System V ABI.

## Development Roadmap

The implementation follows an incremental, feature-driven methodology. Each milestone represents a self-contained, functional compiler supporting a specific subset of the C language.

* [x] **Phase 1: Fundamental Translation** - Support for basic function definitions and integer return statements.
* [ ] **Phase 2: Expressions and Precedence** - Implementation of unary and binary arithmetic operators with correct associativity and binding power.
* [ ] **Phase 3: Storage and Assignment** - Support for local scope variable declarations, stack allocation, and memory assignment.
* [ ] **Phase 4: Control Flow** - Conditional branching, looping constructs, and logical short-circuiting.
* [ ] **Phase 5: Subroutines** - Implementation of the System V AMD64 calling convention for function calls and definitions.
* [ ] **Phase 6: Memory Models** - Support for pointer arithmetic, arrays, and memory referencing/dereferencing.

## Prerequisites

Building the compiler requires the following toolchain:

* C++17 compliant compiler (MSVC 19.14+, GCC 7+, or Clang 5+)
* CMake (Version 3.10 or higher)
* Assembler and Linker (GCC or NASM recommended for processing generated assembly)

## Installation and Build

This project utilizes CMake for cross-platform build configuration.

```bash
# 1. Create and enter build directory
mkdir build
cd build

# 2. Configure the project
cmake ..

# 3. Build the executable
cmake --build .

```

## Usage

The compiler accepts a single `.c` source file and generates an `assembly.s` target file containing x86-64 assembly instructions.

```bash
./ccompiler source_file.c

```

To produce a functional binary on a compatible environment:

```bash
# Assemble and link the output
gcc assembly.s -o binary_out

# Execute the binary
./binary_out

# Verify exit status
echo $?

```