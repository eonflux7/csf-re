#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Releases the resources owned by C_00862d48 itself.
CSF_FUNC(char, __fastcall, C_00862d48_ReleaseResources, 0x006FBB30, (C_00862d48* self, void* edx));
// Base-class teardown (C_008629a4 part of the object).
CSF_FUNC(void, __fastcall, C_008629a4_Destroy, 0x006F8FF0, (C_008629a4* self, void* edx));
// Game heap free (0x6910F0 is alloc; 0x691110 is free).
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Destructor-style teardown of C_00862d48. Sets the vtables, releases its
// resources, runs base cleanup, and frees the block when the low bit of a2 is set.
static int __fastcall C_00862d48_Destroy(C_00862d48* self, void* /*edx*/, char a2)
{
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00862D48);
    CSF_AT(uint32_t, self, 0x0004) = reinterpret_cast<uint32_t>(CSF_VTBL(0x00862D2C));

    C_00862d48_ReleaseResources(self, nullptr);
    C_008629a4_Destroy(reinterpret_cast<C_008629a4*>(self), nullptr);

    if ((a2 & 1) != 0)
        Mem_Free(self);

    return static_cast<int>(reinterpret_cast<uintptr_t>(self));
}

HOOK(0x6FC060, C_00862d48_Destroy, "auto");
