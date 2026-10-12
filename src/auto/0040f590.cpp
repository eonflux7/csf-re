#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, C_0082ddf8_BaseTeardown, 0x0040F0D0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Scalar deleting destructor for C_0082ddf8 (derived from C_0082d410).
// Sets the derived vtable, runs the base-level teardown at 0x40F0D0, restores
// the C_0082d410 vtable, frees the block when bit 0 of a2 is set, and returns
// the object pointer (address of its vtable slot).
static uint32_t* __fastcall C_0082ddf8_ScalarDestructor(void* self, void* /*edx*/, char a2)
{
    *(void***)self = CSF_VTBL(0x0082DDF8);
    C_0082ddf8_BaseTeardown(self, nullptr);
    *(void***)self = CSF_VTBL(0x0082D410);
    if ((a2 & 1) != 0)
        Mem_Free(self);
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x40F590, C_0082ddf8_ScalarDestructor, "auto");
