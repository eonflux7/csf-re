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

CSF_FUNC(char, __fastcall, C_00861a00_ReleaseMediaHandles, 0x00702B80, (C_00861a00* self, void* edx));
CSF_FUNC(char, __fastcall, C_00861a00_ReleaseBaseState_6E0400, 0x006E0400, (void* self, void* edx));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// Scalar deleting destructor of C_00861a00: restores its vtable, releases media handles,
// runs the base-state cleanup, and frees the object when bit 0 of the flag is set.
static void* __fastcall C_00861a00_DestroyAndFreeIfFlag(C_00861a00* self, void* /*edx*/, char a2)
{
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00861A00);

    C_00861a00_ReleaseMediaHandles(self, nullptr);
    C_00861a00_ReleaseBaseState_6E0400(self, nullptr);

    if ((a2 & 1) != 0)
        CoreMemory_Free(self);

    return self;
}

HOOK(0x006E0450, C_00861a00_DestroyAndFreeIfFlag, "auto");
