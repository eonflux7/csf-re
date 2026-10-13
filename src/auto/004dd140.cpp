#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_0083e0d8_ReleaseOwnedObjects, 0x004DCFC0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __fastcall, C_0086c288_ReleaseWaypointGraph, 0x0075E1E0, (C_0086c288* self, void*));

static void C_0083e0d8_ReleaseElementArray(uint32_t elements)
{
    auto* allocation = reinterpret_cast<uint8_t*>(elements) - 4;
    uint32_t remaining = CSF_AT(uint32_t, allocation, 0);

    if (static_cast<int32_t>(remaining - 1) >= 0)
    {
        uintptr_t cursor = elements + 28 * remaining + 4;
        do
        {
            cursor -= 28;
            --remaining;
            CSF_AT(void**, reinterpret_cast<void*>(cursor), 0) =
                CSF_VTBL(0x0082D410);
        }
        while (remaining != 0);
    }

    Mem_Free(allocation);
}

static int __fastcall C_0083e0d8_Destroy(C_0083e0d8* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x0083E0D8);
    C_0083e0d8_ReleaseOwnedObjects(self, nullptr);

    if (self->field_0068)
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_0068));

    self->field_0068 = 0;
    self->field_006c = 0;
    self->field_0070 = 0;

    if (self->field_0054)
    {
        C_0083e0d8_ReleaseElementArray(self->field_0054);
        self->field_0054 = 0;
    }

    if (self->field_0044)
    {
        C_0083e0d8_ReleaseElementArray(self->field_0044);
        self->field_0044 = 0;
    }

    if (self->field_0034)
    {
        C_0083e0d8_ReleaseElementArray(self->field_0034);
        self->field_0034 = 0;
    }

    return C_0086c288_ReleaseWaypointGraph(self, nullptr);
}

HOOK(0x004DD140, C_0083e0d8_Destroy, "auto");
