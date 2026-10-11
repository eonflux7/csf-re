#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Calls virtual slot 0x74 with a2, then passes the result to virtual slot 0x78.
static int __fastcall C_008344f0_ChainVirtualQueries(C_008344f0* self, void* /*edx*/, int a2)
{
    void* intermediate = ((void* (__fastcall*)(void*, void*, int))CSF_AT(void**, self, 0)[0x74 / 4])(self, nullptr, a2);
    return self->vSlot78(intermediate);
}

HOOK(0x4A3180, C_008344f0_ChainVirtualQueries, "auto");
