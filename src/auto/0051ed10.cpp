#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());

static int __fastcall C_00840f6c_ClearFinishedParticleHandles(
    C_00840f6c* self, void* /*edx*/)
{
    void* manager = CSF_AT(void*, reinterpret_cast<void*>(GetValue8B0554()), 0x1FCC);

    if (self->field_0008 != UINT32_MAX)
    {
        void* entry =
            (reinterpret_cast<void* (__fastcall*)(void*, void*, uint32_t)>(
                CSF_AT(void**, manager, 0)[8 / 4]))(
                    manager, nullptr, self->field_0008);
        if (CSF_AT(uint32_t, entry, 0x4))
            self->field_0008 = UINT32_MAX;
    }

    int result = static_cast<int>(self->field_000c);
    if (self->field_000c != UINT32_MAX)
    {
        void* entry =
            (reinterpret_cast<void* (__fastcall*)(void*, void*, uint32_t)>(
                CSF_AT(void**, manager, 0)[8 / 4]))(
                    manager, nullptr, self->field_000c);
        uint32_t finished = CSF_AT(uint32_t, entry, 0x4);
        result = reinterpret_cast<int>(static_cast<uint8_t*>(entry) + 0x4);
        if (finished)
            self->field_000c = UINT32_MAX;
    }
    return result;
}

HOOK(0x0051ED10, C_00840f6c_ClearFinishedParticleHandles, "auto");
