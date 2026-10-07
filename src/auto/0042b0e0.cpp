#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// Sets the byte stored at offset 0x8 of the object and returns that same byte.
// Original: __thiscall, 1 stack argument, callee pops 4 bytes (ret 4).
// Disassembly returns only AL (mov al, [ebp+8]) without clearing the upper
// bytes of EAX, hence the char (byte) return type.
static char __fastcall C_00830438_SetStateByte(C_00830438* self, void* /*edx*/, char value)
{
    self->field_0008 = static_cast<uint8_t>(value);
    return value;
}

HOOK(0x0042B0E0, C_00830438_SetStateByte, "auto");
