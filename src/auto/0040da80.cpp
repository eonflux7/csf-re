#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// 0x0040DA80: returns true when the dword at this+0x50 is zero.
static int __fastcall IsField0050Null(void* self, void* /*edx*/)
{
    return CSF_AT(uint32_t, self, 0x50) == 0;
}

HOOK(0x0040DA80, IsField0050Null, "auto");
