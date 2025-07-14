#include"../include/CPU6502.h"

void LoadProgram(Mem& mem, std::initializer_list<Byte> program, u32 start = 0xFFFC) {
    for (auto byte : program) {
        mem[start++] = byte;
    }
}

int main()
{
    Mem mem;
    CPU cpu;

    // Example Test: Load into Accumulator, Immediate mode
    cpu.Reset(mem);
    LoadProgram(mem, {
        CPU::INS_LDA_IM, 0x80,     // LDA #$80
        CPU::INS_NOP               // NOP
    });
    cpu.Execute(3, mem);

    // Uncomment any of these for more tests

    // Test: LDA Zero Page
    // cpu.Reset(mem);
    // mem[0x00FA] = 229;
    // LoadProgram(mem, { CPU::INS_LDA_ZP, 0xFA });
    // cpu.Execute(3, mem);

    // Test: LDA Zero Page, X
    // cpu.Reset(mem);
    // mem[0x00FA] = 1;
    // mem[0x00FB] = 2;
    // cpu.X = 1;
    // LoadProgram(mem, { CPU::INS_LDA_ZPX, 0xFA });
    // cpu.Execute(4, mem);

    // Test: LDX Immediate
    // cpu.Reset(mem);
    // LoadProgram(mem, { CPU::INS_LDX_IM, 42 });
    // cpu.Execute(2, mem);

    // Test: LDY Zero Page
    // cpu.Reset(mem);
    // mem[0x0025] = 64;
    // LoadProgram(mem, { CPU::INS_LDY_ZP, 0x25 });
    // cpu.Execute(3, mem);

    // Test: JSR
    // cpu.Reset(mem);
    // mem[0x4342] = CPU::INS_LDA_IM;
    // mem[0x4343] = 21;
    // LoadProgram(mem, {
    //     CPU::INS_JSR, 0x42, 0x43  // JSR $4342 (little endian)
    // });
    // cpu.Execute(9, mem);

    return 0;
}
