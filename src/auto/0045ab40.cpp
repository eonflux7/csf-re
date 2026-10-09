#include <cstdint>
#include "globals.h"
#include "hook.h"

// Reads a pointer-sized field at base+off (base is a raw object address).
static uint32_t ReadPointerFieldAt(uint32_t base, uint32_t off) {
    return CSF_AT(uint32_t, reinterpret_cast<void*>(base), off);
}

// Operand chain: operand -> +4 -> record, record +4 holds the float at +4.
static float ReadOperandValue(uint32_t operand) {
    uint32_t record = ReadPointerFieldAt(operand, 4);
    return CSF_AT(float, reinterpret_cast<void*>(record), 4);
}

// Evaluates a binary arithmetic node (op selected by byte at node->+4->+4+1)
// and stores the result into result[1] of the destination object at a1+4.
// Opcodes: 0 = add, 1 = subtract, 2 = multiply, 3 = divide. Others are a no-op.
static float* __stdcall ApplyArithmeticOpToResult(int a1, int a2, float* a3) {
    (void)a2;

    uint32_t node = reinterpret_cast<uint32_t>(a3);
    uint32_t opRecord = ReadPointerFieldAt(ReadPointerFieldAt(node, 4), 4);
    int opcode = CSF_AT(int8_t, reinterpret_cast<void*>(opRecord), 1);

    float* result = a3;
    if (static_cast<uint32_t>(opcode) > 3u) {
        return result;
    }

    uint32_t leftOperand = ReadPointerFieldAt(node, 0);
    uint32_t rightOperand = ReadPointerFieldAt(node, 8);
    result = reinterpret_cast<float*>(ReadPointerFieldAt(static_cast<uint32_t>(a1), 4));

    float value = 0.0f;
    switch (opcode) {
    case 0:
        value = ReadOperandValue(rightOperand) + ReadOperandValue(leftOperand);
        break;
    case 1:
        value = ReadOperandValue(leftOperand) - ReadOperandValue(rightOperand);
        break;
    case 2:
        value = ReadOperandValue(rightOperand) * ReadOperandValue(leftOperand);
        break;
    case 3:
        value = ReadOperandValue(leftOperand) / ReadOperandValue(rightOperand);
        break;
    default:
        return result;
    }

    result[1] = value;
    return result;
}

HOOK(0x0045AB40, ApplyArithmeticOpToResult, "auto");
