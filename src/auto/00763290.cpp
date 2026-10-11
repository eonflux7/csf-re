#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

// Original: 0x00510FF0, __fastcall (ecx = this), releases the resources attached to the block at field_0008.
CSF_FUNC(int, __fastcall, C_0086f128_ReleaseAttachedData, 0x00510FF0, (void* self, void* edx));

// Original: 0x00691110, game heap free (__cdecl).
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Original: 0x00763290 (C_0086f128 destructor body). ret 4 => one stack argument (a2, flags; bit 0 = free this).
static int** __fastcall C_0086f128_Destroy(C_0086f128* self, void* /*edx*/, char flags)
{
    uint32_t attachedBlock = CSF_AT(uint32_t, self, 0x8);

    *reinterpret_cast<void***>(self) = CSF_VTBL(0x0086F128);

    if (attachedBlock) {
        C_0086f128_ReleaseAttachedData(reinterpret_cast<void*>(attachedBlock), nullptr);
        Mem_Free(reinterpret_cast<void*>(attachedBlock));
        CSF_AT(uint32_t, self, 0x8) = 0;
    }

    if ((flags & 1) != 0) {
        Mem_Free(self);
    }

    return reinterpret_cast<int**>(self);
}

HOOK(0x763290, C_0086f128_Destroy, "auto");
