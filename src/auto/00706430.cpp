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

CSF_FUNC(unsigned int, __fastcall, C_00861d48_ReleaseOwnedEntries_6E6C70, 0x006E6C70, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_00861d48_Destroy_6E70A0, 0x006E70A0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// Scalar deleting destructor of C_00863260: restores its vtable, releases the owned entries,
// runs the base-class destroy step, and frees the object when bit 0 of the flag is set.
static unsigned int* __fastcall C_00863260_DestroyAndFreeIfFlag(void* self, void* /*edx*/, char a2)
{
    *static_cast<void***>(self) = CSF_VTBL(0x00863260);

    C_00861d48_ReleaseOwnedEntries_6E6C70(self, nullptr);
    C_00861d48_Destroy_6E70A0(self, nullptr);

    if ((a2 & 1) != 0)
        CoreMemory_Free(self);

    return static_cast<unsigned int*>(self);
}

HOOK(0x00706430, C_00863260_DestroyAndFreeIfFlag, "auto");
