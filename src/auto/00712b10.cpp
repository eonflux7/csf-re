#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));
CSF_FUNC(int, __cdecl, AudioEffect_ReleaseHandle_7DEF60, 0x007DEF60, (void* handle, void* callback, int flags));
CSF_FUNC(int, __cdecl, AudioEffect_ReleaseHandle_7DF740, 0x007DF740, (int a1, int a2));

// Destroys a C_00863dd0 audio effect object: releases its handle through both
// AudioEffect_ReleaseHandle variants, resets the fields and vtables, and frees
// the block when the low bit of freeFlag is set (deleting-destructor style).
static uint32_t* __fastcall C_00863dd0_Destroy(C_00863dd0* self, void* /*edx*/, char freeFlag)
{
    uint32_t handleValue = self->field_0004;
    *(void***)self = CSF_VTBL(0x00863DD0);
    AudioEffect_ReleaseHandle_7DF740(static_cast<int>(handleValue), 0);

    self->field_000c = 0;
    AudioEffect_ReleaseHandle_7DEF60(reinterpret_cast<void*>(self->field_0004), nullptr, 0);

    self->field_0004 = 0;
    *(void***)self = CSF_VTBL(0x008639F0);

    if ((freeFlag & 1) != 0)
        CoreMemory_Free(self);

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x712B10, C_00863dd0_Destroy, "auto");
