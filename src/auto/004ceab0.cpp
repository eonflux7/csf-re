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

CSF_FUNC(int, __fastcall, C_0083b958_Destroy, 0x004D1140, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Destroys a C_0083ad48 object: releases its owned buffer, runs the base
// destructor, and frees the object itself when bit 0 of freeFlag is set.
static uint32_t* __fastcall C_0083ad48_Destroy(C_0083ad48* self, void* /*edx*/, char freeFlag)
{
    uint32_t ownedBuffer = self->field_004c;

    self->vtbl = CSF_VTBL(0x0083AD48);
    self->field_0004 = (uint32_t)CSF_VTBL(0x0083AD28);

    if (ownedBuffer)
    {
        Mem_Free(reinterpret_cast<void*>(static_cast<uintptr_t>(ownedBuffer)));
        self->field_004c = 0;
    }

    C_0083b958_Destroy(self, nullptr);

    if ((freeFlag & 1) != 0)
        Mem_Free(self);

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x004CEAB0, C_0083ad48_Destroy, "auto");
