#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static void __fastcall S_006d2390_ClearLights(S_006d2390* self, void* /*edx*/)
{
    if (!self->field_0000)
        return;

    const uint32_t begin = self->field_0008;
    uint32_t remaining = begin
        ? static_cast<uint32_t>(
              static_cast<int32_t>(self->field_000c - begin) >> 4)
        : 0;
    uint32_t offset = 0;

    while (remaining != 0)
    {
        void* entries = reinterpret_cast<void*>(self->field_0008 + offset);
        void* owner = reinterpret_cast<void*>(self->field_0038);
        const uint32_t light = CSF_AT(uint32_t, entries, 0x0C);

        reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t)>(
            CSF_AT(void**, owner, 0)[8 / 4])(owner, nullptr, light);

        offset += 16;
        --remaining;
    }

    if (self->field_0008)
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_0008));
    self->field_0008 = 0;
    self->field_000c = 0;
    self->field_0010 = 0;

    if (self->field_0018)
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_0018));
    self->field_0018 = 0;
    self->field_001c = 0;
    self->field_0020 = 0;

    if (self->field_0028)
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_0028));
    self->field_0028 = 0;
    self->field_002c = 0;
    self->field_0030 = 0;

    self->field_0000 = 0;
}

HOOK(0x006D2390, S_006d2390_ClearLights, "auto");
