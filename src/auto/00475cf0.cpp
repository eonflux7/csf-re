#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall C_008316d8_SetFiringCount(
    C_008316d8* self, void* /*edx*/, char doubled)
{
    if (doubled)
    {
        const uint32_t definition = self->field_0040;
        self->field_001c =
            2 * CSF_AT(uint32_t, reinterpret_cast<void*>(definition), 0x104);
        return static_cast<int32_t>(definition);
    }

    if (static_cast<int32_t>(self->field_0020) > 1)
    {
        const uint32_t count =
            CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0040), 0x104);
        void* target = CSF_AT(void**, self, 0)[0x78 / 4];
        self->field_001c = 2 * count;
        return reinterpret_cast<int(__fastcall*)(void*, void*, int)>(target)(
            self, nullptr, 2);
    }

    const uint32_t count =
        CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0040), 0x104);
    void* target = CSF_AT(void**, self, 0)[0x78 / 4];
    self->field_001c = count;
    return reinterpret_cast<int(__fastcall*)(void*, void*, int)>(target)(
        self, nullptr, 1);
}

HOOK(0x00475CF0, C_008316d8_SetFiringCount, "auto");
