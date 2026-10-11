#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Base-class cleanup (C_0082d5e0 / C_0082d410 part of the object).
CSF_FUNC(unsigned int, __fastcall, C_0082d5e0_BaseCleanup, 0x00408A40, (void* self, void* edx));
// Releases the resources owned by C_0082d8a0 itself.
CSF_FUNC(void, __fastcall, C_0082d8a0_ReleaseOwnedResources, 0x00406090, (void* self, void* edx));
// Game heap free (0x6910F0 is alloc; 0x691110 is free).
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Destructor-style teardown of C_0082d8a0. Sets the vtables, runs base cleanup
// and resource release, and frees the block when the low bit of a2 is set.
static uint32_t* __fastcall C_0082d8a0_Destroy(C_0082d8a0* self, void* /*edx*/, char a2)
{
    self->vtbl = CSF_VTBL(0x0082D8A0);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D880));

    C_0082d5e0_BaseCleanup(self, nullptr);
    C_0082d8a0_ReleaseOwnedResources(self, nullptr);

    if ((a2 & 1) != 0)
        Mem_Free(self);

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x409F80, C_0082d8a0_Destroy, "auto");
