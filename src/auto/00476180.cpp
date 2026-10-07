#include <cstdint>

#include "globals.h"
#include "core/memory.h"
#include "hook.h"

// __thiscall(int a2): ecx = this, one 32-bit argument on the stack (ret 4).
// Stores a2 into the object's field at offset 0x20 and returns a2.
static int __fastcall C_Unknown_StoreField20(void* self, void* /*edx*/, int value)
{
    CSF_AT(int, self, 0x20) = value;
    return value;
}

HOOK(0x476180, C_Unknown_StoreField20, "auto");
