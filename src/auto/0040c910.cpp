#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// __thiscall method (ecx = this, edx unused): returns whether the dword at +0x90 is zero.
static int __fastcall IsField90Null(void* self, void* /*edx*/)
{
    uint32_t value = CSF_AT(uint32_t, self, 0x90);
    return value == 0;
}

HOOK(0x40C910, IsField90Null, "auto");
