#pragma once
#include<iostream>
#include<iomanip>

using Byte = unsigned char; // 'using' creates type aliases // 8bits
using Word = unsigned short; // 2 Bytes
using u32 = unsigned int; // 3 Bytes or 32 Bits

struct Mem
{
    static constexpr u32 MAX_MEM = 1024 * 64; // Evaluated at compile time
    Byte Data[MAX_MEM];

    // constexpr provides typechecking and encapsulation over #define
    // It ensures MAX_MEM is treated as a constant with a defined type

    void initialise()
    {
        for (u32 i = 0; i < MAX_MEM; i++)
        {
            Data[i] = 0;
        }
    }

// Memory Structure : |0000---Zero-Page---00FF|0100-----Stack-----01FF|0200----Memory----FFFF|
 
    // Read 1 byte
    Byte operator[](u32 Address) const // 'const' version, used for reading memory
    {
        // Address is < MAX_MEM
        return Data[Address];
    }
    // Write 1 byte
    Byte &operator[](u32 Address)   // non-const version used to write memory
    {
        // assert here Address is < MAX_MEM
        return Data[Address];
    }

    // Operator overloading functions are used to provide a convinient way to read
    // and write memory contents using the 'Data' array in the 'Mem' structure

    //  Write 2 bytes
    void WriteWord(u32 &Cycles, Word Value, u32 Address)
    {
        Data[Address] = Value & 0xFF; // Writes lower 8 bits
        Data[Address + 1] = (Value >> 8); // Writes upper 8 bits
        Cycles -= 2;
    }

    void showMemory()
    {
        for (u32 i = 0; i < MAX_MEM; i++)
        {
            if (Data[i] != 0)
            {
                std::cout << "Address: 0x" << std::setw(4) << std::setfill('0') << std::hex << i
                          << " --------- Value: 0x" << std::setw(2) << std::setfill('0') << std::hex << +Data[i]
                          << " --> " << std::dec << +Data[i] << '\n';

            }
        }
        std::cout<<"\n";
    }
};

struct CPU
{
    Word PC; // program counter
    Word SP; // Stack pointer

    Byte A, X, Y; // registers Accumulator(A), Index(X and Y)

    Byte C : 1; // Carry status flag : Used for carry and borrow operations
    Byte Z : 1; // Zero status flag : Set when result of previous opertion was zero
    Byte I : 1; // Interrupt status flag : Determines whether interrupts are enabled or disabled
    Byte D : 1; // Decimal status flag : Controls whether cpu is operating on decimal or binary
    Byte B : 1; // Break status flag : set when an interrupt request(BRK instruction) is encountered
    Byte V : 1; // Overflow status flag : When arithmatic overflow occours
    Byte N : 1; // Negative status flag : when result of an operation is negative

    // " : " defines bit fields. 'Byte C : 1' defines a bitfield for C of width 1

    void ShowRegisters(u32 Cycles)
    {
        std::cout << "Accumulator: " << +A
                  << " Registers (X: " << +X << " Y: " << +Y << ") "
                  << "StackPointer: 0x" << std::setw(4) << std::setfill('0') << std::hex << SP
                  << " ProgramCounter: 0x" << std::setw(4) << std::setfill('0') << std::hex << PC << '\n';
        std::cout << "Flags -> "
                  << "Carry: " << +C << " "
                  << "Zero: " << +Z << " "
                  << "Interrupt: " << +I << " "
                  << "Decimal: " << +D << " "
                  << "Break: " << +B << " "
                  << "oVerflow: " << +V << " "
                  << "Negative: " << +N << '\n';
        std::cout << "Cycles left: " << std::dec << Cycles << '\n';
    }

    void Reset(Mem &memory)
    {
        PC = 0xFFFC;
        SP = 0x0100;
        C = D = Z = I = B = V = N = 0;
        A = X = Y = 0;
        memory.initialise();
    }

    Byte FetchByte(u32 &Cycles, Mem &memory)
    {
        Byte Data = memory[PC];
        PC++;
        Cycles--;
        return Data;
    }

    Word Fetchword(u32 &Cycles, Mem &memory)
    {
        // Because 6502 is little endian
        Word Data = memory[PC];
        PC++;

        Data |= (memory[PC] << 8); 
        PC++;
        Cycles -= 2;
        return Data;
    }

    //  If you wanted to handle endianness
    //  you would have to swap bytes here
    //  if ( PLATFORM_BIG_ENDIAN )
    //      SwapBytesInWord(Data)

    Byte ReadByte(Byte Address, u32 &Cycles, Mem &memory)
    {
        Byte Data = memory[Address];
        Cycles--;
        return Data;
    }

    // opcodes
    static constexpr Byte
        INS_LDA_IM = 0xA9,  // Load into accumulator Immidiate mode
        INS_LDA_ZP = 0xA5,  // Load into accumulator ZeroPage mode
        INS_LDA_ZPX = 0xB5, // Load into accumulator ZeroPage + X register mode
        INS_LDX_IM = 0xA2,  // Load into x register Immidiate mode
        INS_LDX_ZP = 0xA6,  // Load into x register ZeroPage mode
        INS_LDY_IM = 0xA0,  // Load into y register Immidiate mode
        INS_LDY_ZP = 0xA4,  // Load into y register ZeroPage mode
        INS_LSR_ACC = 0x4A, // Logical shift right Accumulator mode
        INS_LSR_ZP = 0x46,  // Logical shift right ZeroPage mode
        INS_ORA_IM = 0x09,  // OR operation in accumulator immidiate mode
        INS_ORA_ZP = 0x05,  // OR operation in accumulator ZeroPage mode
        INS_PHA = 0x48,     // Push contents of accumulator on the stack
        INS_JSR = 0xA8,     // Jump to subroutine
        INS_NOP = 0xEA;     // No operation

    void SetNZFlags(Byte Value)
    {
        Z = (Value == 0);
        N = (Value & 0x80) > 0;
    }

    Byte instructions;

    void Show(u32 Cycles, Mem &memory)
    {
        std::cout << "Executed, opcode: 0x" << std::hex << std::setw(2) << std::setfill('0') << +instructions << '\n';
        ShowRegisters(Cycles);
        memory.showMemory();
    }

    void Execute(u32 Cycles, Mem &memory)
    {
        while (Cycles > 0)
        {
            instructions = FetchByte(Cycles, memory);
            switch (instructions)
            {
            case INS_LDA_IM:
            {
                Byte Value = FetchByte(Cycles, memory);
                A = Value;
                SetNZFlags(A);
                Show(Cycles, memory);
            }
            break;
            case INS_LDA_ZP:
            {
                Byte ZeroPageAddress = FetchByte(Cycles, memory);
                A = ReadByte(ZeroPageAddress, Cycles, memory);
                SetNZFlags(A);
                Show(Cycles, memory);
            }
            break;
            case INS_LDA_ZPX:
            {
                Byte ZeroPageAddress = FetchByte(Cycles, memory);
                ZeroPageAddress += X;
                Cycles--;
                A = ReadByte(ZeroPageAddress, Cycles, memory);
                SetNZFlags(A);
                Show(Cycles, memory);
            }
            break;
            case INS_LDX_IM:
            {
                Byte Value = FetchByte(Cycles, memory);
                X = Value;
                SetNZFlags(X);
                Show(Cycles, memory);
            }
            break;
            case INS_LDX_ZP:
            {
                Byte ZeroPageAddress = FetchByte(Cycles, memory);
                X = ReadByte(ZeroPageAddress, Cycles, memory);
                SetNZFlags(X);
                Show(Cycles, memory);
            }
            break;

            case INS_LDY_IM:
            {
                Byte Value = FetchByte(Cycles, memory);
                Y = Value;
                SetNZFlags(Y);
                Show(Cycles, memory);
            }
            break;

            case INS_LDY_ZP:
            {
                Byte ZeroPageAddress = FetchByte(Cycles, memory);
                Y = ReadByte(ZeroPageAddress, Cycles, memory);
                SetNZFlags(Y);
                Show(Cycles, memory);
            }
            break;

            case INS_LSR_ACC:
            {
                C = A & 0x01;
                A >>= 1;
                Z = (A == 0);
                N = 0;
                Show(Cycles, memory);
            }
            break;

            case INS_LSR_ZP:
            {
                Byte ZeroPageAddress = FetchByte(Cycles, memory);
                Byte Value = ReadByte(ZeroPageAddress, Cycles, memory);
                C = Value & 0x01;
                Value >>= 1;
                Z = (Value == 0);
                N = 0;
                memory[ZeroPageAddress] = Value;
                Show(Cycles, memory);
            }
            break;

            case INS_NOP:
            {
                Show(Cycles, memory);
            }
            break;

            case INS_ORA_IM:
            {
                Byte Value = FetchByte(Cycles, memory);
                A |= Value;
                SetNZFlags(A);
                Show(Cycles, memory);
            }
            break;

            case INS_ORA_ZP:
            {
                Byte ZeroPageAddress = FetchByte(Cycles, memory);
                Byte Value = ReadByte(ZeroPageAddress, Cycles, memory);
                A |= Value;
                SetNZFlags(A);
                Show(Cycles, memory);
            }
            break;

            case INS_PHA:
            {
                memory[SP] = A;
                SP--;
                Show(Cycles, memory);
            }
            break;
            case INS_JSR:
            {
                Word SubAddress = Fetchword(Cycles, memory);
                memory.WriteWord(Cycles, PC - 1, SP);
                SP+=2;
                PC = SubAddress;
                Cycles -= 2;
                Show(Cycles, memory);
            }
            break;
            default:
            {
                std::cout<<"Instruction not handled, opcode : %04X\n", instructions;
            }
            break;
            }
        }
    }
};
