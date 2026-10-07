#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// 0x00538970: trivial accessor returning the 32-bit value at offset 0x2C.
// Disassembly: mov eax, [ecx+0x2c] / ret 8  (two __thiscall stack args, both unused)
static int __fastcall C_00842150_GetField002c(C_00842150* self, void* /*edx*/, int /*a2*/, int /*a3*/)
{
    return static_cast<int>(self->field_002c);
}

HOOK(0x00538970, C_00842150_GetField002c, "auto");
