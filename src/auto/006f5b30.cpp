#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Stores the given 32-bit value into this+0x38 and returns it.
// __thiscall: ecx = this, arg0 on the stack (ret 4).
static int __fastcall C_6F5B30_SetField38(void* self, void* /*edx*/, int value)
{
    CSF_AT(int, self, 0x38) = value;
    return value;
}

HOOK(0x006F5B30, C_6F5B30_SetField38, "auto");
