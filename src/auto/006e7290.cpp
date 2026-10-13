#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Frees a block allocated by the game heap (cdecl, caller pops nothing extra).
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// Base-class cleanup (C_0082d410 side), __fastcall with ecx=this, edx unused.
CSF_FUNC(unsigned int, __fastcall, C_0082d410_ReleaseBaseState, 0x006E6C70, (void* self, void* edx));

// Destructor of C_00861d48: resets vtable, releases base state, frees the two owned
// buffers (offsets 0x3C and 0x28), restores the base vtable, optionally frees the object
// (bit 0 of a2 = delete flag). Returns this. __thiscall, ret 4.
static int __fastcall C_00861d48_Destroy(C_00861d48* self, void* /*edx*/, char a2)
{
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00861D48);

    C_0082d410_ReleaseBaseState(self, nullptr);

    if (CSF_AT(void*, self, 0x3C))
    {
        CoreMemory_Free(CSF_AT(void*, self, 0x3C));
        CSF_AT(void*, self, 0x3C) = nullptr;
    }

    if (CSF_AT(void*, self, 0x28))
    {
        CoreMemory_Free(CSF_AT(void*, self, 0x28));
        CSF_AT(void*, self, 0x28) = nullptr;
    }

    *reinterpret_cast<void***>(self) = CSF_VTBL(0x0082D410);

    if ((a2 & 1) != 0)
    {
        CoreMemory_Free(self);
    }

    return reinterpret_cast<int>(self);
}

HOOK(0x006E7290, C_00861d48_Destroy, "auto");
