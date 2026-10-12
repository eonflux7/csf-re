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

CSF_FUNC(int, __fastcall, C_0082deb8_ReleaseChildBase_41E110, 0x0041E110, (C_0082e520* self, void*));
CSF_FUNC(int, __fastcall, C_0082dc88_ReleaseSecondaryResources_40F760, 0x0040F760, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Scalar deleting destructor of C_0082dc88: restores its vtable, releases the base-class
// child state, runs the secondary cleanup, and frees the object when bit 0 of the flag is set.
static int* __fastcall C_0082dc88_DestroyAndFreeIfFlag(void* self, void* /*edx*/, char a2)
{
    *static_cast<void***>(self) = CSF_VTBL(0x0082DC88);

    C_0082deb8_ReleaseChildBase_41E110(reinterpret_cast<C_0082e520*>(self), nullptr);
    C_0082dc88_ReleaseSecondaryResources_40F760(self, nullptr);

    if ((a2 & 1) != 0)
        Mem_Free(self);

    return static_cast<int*>(self);
}

HOOK(0x0040D400, C_0082dc88_DestroyAndFreeIfFlag, "auto");
