#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_00840d54_ReleaseBaseState, 0x00519850, (void* self, void* edx));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

static uint32_t* __fastcall C_00840d64_Destroy(C_00840d64* self, void* /*edx*/, char flags)
{
    self->vtbl = CSF_VTBL(0x00840D64);
    if (self->field_0018)
    {
        CoreMemory_Free(reinterpret_cast<void*>(self->field_0018));
        self->field_0018 = 0;
    }

    C_00840d54_ReleaseBaseState(self, nullptr);

    if ((flags & 1) != 0)
        CoreMemory_Free(self);

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x00519950, C_00840d64_Destroy, "auto");
