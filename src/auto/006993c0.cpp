#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

static int __fastcall C_0085e720_Destroy(C_0085e720* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x0085E720);

    if (self->field_0008)
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_0008));

    self->field_0008 = 0;
    self->field_000c = 0;
    self->field_0010 = 0;

    if (self->field_0058)
    {
        CoreMemory_Free(reinterpret_cast<void*>(self->field_0058));
        self->field_0058 = 0;
    }

    if (self->field_0054 >= 0x10 && self->field_0040)
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_0040));

    self->field_0054 = 15;
    self->field_0050 = 0;
    *reinterpret_cast<std::uint8_t*>(&self->field_0040) = 0;

    if (self->field_0038 >= 0x10 && self->field_0024)
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_0024));

    self->field_0038 = 15;
    self->field_0034 = 0;
    *reinterpret_cast<std::uint8_t*>(&self->field_0024) = 0;

    int result = static_cast<int>(self->field_0008);
    if (result)
        result = Mem_FreeDebug(reinterpret_cast<void*>(self->field_0008));

    self->field_0008 = 0;
    self->field_000c = 0;
    self->field_0010 = 0;
    return result;
}

HOOK(0x006993C0, C_0085e720_Destroy, "auto");
