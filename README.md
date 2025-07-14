# 6502 CPU Emulator in C++

A lightweight emulator for the legendary **MOS 6502 8-bit microprocessor**, implemented in modern C++.  
This project was designed to simulate how a real CPU works at the instruction level, including memory access, register manipulation, and flag behavior.

> Built from scratch as a personal learning project by an electronics engineer to deeply understand how microprocessors function internally.

---

## Features

- Accurate simulation of key **6502 instructions**:
  - `LDA` – Load Accumulator (Immediate, Zero Page, Zero Page,X)
  - `LDX`, `LDY` – Load Index Registers
  - `ORA` – Logical OR with Accumulator
  - `LSR` – Logical Shift Right (Accumulator and Zero Page)
  - `PHA` – Push Accumulator to Stack
  - `JSR` – Jump to Subroutine
  - `NOP` – No Operation
- Emulates CPU cycle counting
- Zero Page, Stack, and Program Memory modeled
- Status flags: Carry, Zero, Interrupt, Decimal, Break, Overflow, Negative
- Structured, object-oriented design
- Modular header for easy reuse or extension

---

## Code Structure
6502-emulator
> CPU6502.h # Core module: CPU + Memory structs
> main.cpp # Test runner and usage examples
> README.md # Project documentation (you are here)


### `CPU6502.h` Overview

Defines two core components:

#### `struct Mem`
- Represents 64KB memory space.
- Allows byte-wise and word-wise read/write.
- Memory layout:
  - `0x0000 - 0x00FF` → Zero Page
  - `0x0100 - 0x01FF` → Stack
  - `0x0200 - 0xFFFF` → General Memory

**Key methods**:
- `initialise()` – Zeros out memory
- `operator[]` – Read/write memory via overloaded `[]`
- `WriteWord()` – Write 16-bit data
- `showMemory()` – Print non-zero memory locations

#### `struct CPU`
Simulates the internal state and behavior of a 6502 microprocessor.

**Core members**:
- **Registers**: `A`, `X`, `Y` (accumulator and index registers)
- **Pointers**: `PC` (program counter), `SP` (stack pointer)
- **Flags**: `C`, `Z`, `I`, `D`, `B`, `V`, `N` as bitfields
- **Cycle-aware execution**

**Key methods**:
- `Reset(Mem&)` – Sets CPU to starting state, clears memory
- `FetchByte()`, `FetchWord()` – Retrieve bytes or words from memory, advancing PC
- `ReadByte()` – Non-PC based memory read
- `Execute()` – Main loop executing instructions based on opcode
- `SetNZFlags()` – Set Zero and Negative flags based on result
- `ShowRegisters()` / `Show()` – Debug output of CPU state

---

## Installation

You’ll need a modern C++ compiler (e.g., `g++`, `clang++`, or MSVC).

### 🐧 Linux / 🪟 Windows (with MinGW)

```bash
git clone https://github.com/ArkChaudhary/6502-Emulator.git
cd 6502-Emulator
g++ main.cpp -o emulator
./emulator

