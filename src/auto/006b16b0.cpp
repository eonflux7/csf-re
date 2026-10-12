#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(char, __fastcall, C_00860104_BaseTeardown, 0x006E2C70, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Scalar deleting destructor for C_00860120 (derived from C_00860104).
// Sets the derived vtable, runs the base-level teardown at 0x6E2C70, restores
// the C_00860104 vtable, frees the block when bit 0 of a2 is set, and returns
// the object pointer (address of its vtable slot).
static uint32_t* __fastcall C_00860120_ScalarDestructor(void* self, void* /*edx*/, char a2)
{
    *(void***)self = CSF_VTBL(0x00860120);
    C_00860104_BaseTeardown(self, nullptr);
    *(void***)self = CSF_VTBL(0x00860104);
    if ((a2 & 1) != 0)
        Mem_Free(self);
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x6B16B0, C_00860120_ScalarDestructor, "auto");
