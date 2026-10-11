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

// Subobject cleanup for the member embedded at +0x18 (S_0050f230).
CSF_FUNC(int, __fastcall, S_0050f230_Cleanup, 0x00510290, (void* self, void* edx));
// Base-class cleanup (C_00840704), run on the whole object.
CSF_FUNC(int, __fastcall, C_00840704_Cleanup, 0x005105F0, (void* self, void* edx));
// Game heap free.
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Destructor body for C_00840724. Takes one stack argument (flags byte; bit 0 = free storage).
static uint32_t* __fastcall C_00840724_Destroy(C_00840724* self, void* /*edx*/, char flags)
{
    self->vtbl = CSF_VTBL(0x00840724);

    S_0050f230_Cleanup(reinterpret_cast<uint8_t*>(self) + 0x18, nullptr);
    C_00840704_Cleanup(self, nullptr);

    if ((flags & 1) != 0) {
        Mem_Free(self);
    }

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x005106D0, C_00840724_Destroy, "auto");
