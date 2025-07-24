# 6502 CPU Emulator in C++

In this project, I built an emulator for **MOS 6502 8-bit microprocessor**, implemented in modern C++.  
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
```

---

## Creating Programs

6502 programs are written by directly placing opcodes and data into memory. The program starts at address `0xFFFC`, which is where the 6502 looks for the reset vector (start of execution).
Here’s how to write a test using the built-in `LoadProgram` helper (define this in your `main.cpp`):

```cpp
void LoadProgram(Mem& mem, std::initializer_list<Byte> program, u32 start = 0xFFFC) {

    for (auto byte : program) {
        mem[start++] = byte;
    }
}
```

### Example Program

This loads the value `0x80` into the accumulator and executes a `NOP`:

```cpp
int main()
{
    Mem mem;
    CPU cpu;

    cpu.Reset(mem);

    LoadProgram(mem, {
        CPU::INS_LDA_IM, 0x80,   // LDA #$80
        CPU::INS_NOP             // NOP
    });

    cpu.Execute(3, mem);
    return 0;
}
```
### Output

```bash
Executed, opcode: 0xa9
Accumulator: 80 Registers (X: 0 Y: 0) StackPointer: 0x0100 ProgramCounter: 0xfffe
Flags -> Carry: 0 Zero: 0 Interrupt: 0 Decimal: 0 Break: 0 oVerflow: 0 Negative: 1
Cycles left: 1
Address: 0xfffc --------- Value: 0xa9 --> 169
Address: 0xfffd --------- Value: 0x80 --> 128
Address: 0xfffe --------- Value: 0xea --> 234

Executed, opcode: 0xea
Accumulator: 80 Registers (X: 0 Y: 0) StackPointer: 0x0100 ProgramCounter: 0xffff
Flags -> Carry: 0 Zero: 0 Interrupt: 0 Decimal: 0 Break: 0 oVerflow: 0 Negative: 1
Cycles left: 0
Address: 0xfffc --------- Value: 0xa9 --> 169
Address: 0xfffd --------- Value: 0x80 --> 128
Address: 0xfffe --------- Value: 0xea --> 234
```

---

## Why I built this project

As an electronics engineer, I wanted a hands-on way to explore the internal mechanics of a microprocessor.
This project gave me the opportunity to:

- Study the 6502 Instruction Set Architecture
- Understand microprocessor internals like registers, memory mapping, stack behavior, and flag logic
- Sharpen my C++ skills — including structs, pointers, memory management, bitfields, and modular design
- Develop an intuitive grasp of how CPU instructions are fetched, decoded, and executed
- This emulator served as a deep dive into both low-level computing and systems programming.
