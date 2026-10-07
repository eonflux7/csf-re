#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// 0x0050F6D0 (6 bytes):
//   mov eax, 0x0082E65C
//   ret
// Returns the address of the constant byte at 0x0082E65C (an empty/constant
// string in the original data segment). No `this` state is used or modified.
static const char* __fastcall C_0082fbe8_GetConstantName(C_0082fbe8* /*self*/, void* /*edx*/)
{
    return reinterpret_cast<const char*>(0x0082E65C);
}

HOOK(0x0050F6D0, C_0082fbe8_GetConstantName, "auto");
