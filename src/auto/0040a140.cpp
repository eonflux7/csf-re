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

// Base-class cleanup (C_0082d5e0 / C_0082d410 part of the object).
CSF_FUNC(unsigned int, __fastcall, C_0082d5e0_BaseCleanup, 0x00408A40, (void* self, void* edx));
// Releases the resources owned by C_0082d970 itself.
CSF_FUNC(void, __fastcall, C_0082d970_ReleaseOwnedResources, 0x00406090, (void* self, void* edx));
// Game heap free (0x6910F0 is alloc; 0x691110 is free).
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Destructor-style teardown of C_0082d970. Sets the vtables, runs base cleanup
// and resource release, and frees the block when the low bit of a2 is set.
static int* __fastcall C_0082d970_Destroy(C_0082d970* self, void* /*edx*/, char a2)
{
    self->vtbl = CSF_VTBL(0x0082D970);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x0082D950));

    C_0082d5e0_BaseCleanup(self, nullptr);
    C_0082d970_ReleaseOwnedResources(self, nullptr);

    if ((a2 & 1) != 0)
        Mem_Free(self);

    return reinterpret_cast<int*>(self);
}

HOOK(0x40A140, C_0082d970_Destroy, "auto");
