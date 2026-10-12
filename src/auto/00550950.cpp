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

CSF_FUNC(int, __fastcall, C_008445f4_ReleaseChildState_54ED50, 0x0054ED50, (void* self, void* edx));
CSF_FUNC(void, __fastcall, MpSpawnPoints_DestroyEmbeddedObject, 0x0073F710, (C_00867768* self, void*));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Scalar deleting destructor of C_008445f4: restores its vtable, releases the child state,
// destroys the embedded spawn-point object, and frees the object when bit 0 of the flag is set.
static int* __fastcall C_008445f4_DestroyAndFreeIfFlag(void* self, void* /*edx*/, char a2)
{
    *static_cast<void***>(self) = CSF_VTBL(0x008445F4);

    C_008445f4_ReleaseChildState_54ED50(self, nullptr);
    MpSpawnPoints_DestroyEmbeddedObject(static_cast<C_00867768*>(self), nullptr);

    if ((a2 & 1) != 0)
        Mem_Free(self);

    return static_cast<int*>(self);
}

HOOK(0x00550950, C_008445f4_DestroyAndFreeIfFlag, "auto");
