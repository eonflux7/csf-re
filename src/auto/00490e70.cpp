#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
static CSF_FUNC(uint32_t*, __cdecl, CreateSetupValue, 0x004E0FE0, (int a1, int a2, int a3));

static int __fastcall C_00833908_SetSetupValue(
    C_00833908* self, void* /*edx*/, int setupType, int setupValue)
{
    void* previous = reinterpret_cast<void*>(self->field_0038);
    if (previous)
    {
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, previous, 0)[0])(previous, nullptr, 1);
        self->field_0038 = 0;
    }

    uint32_t created = reinterpret_cast<uint32_t>(
        CreateSetupValue(setupType, reinterpret_cast<int>(self), setupValue));
    self->field_0038 = created;

    uint32_t result = created;
    if (created)
    {
        auto* values = reinterpret_cast<S_0041e5f0*>(
            reinterpret_cast<uint8_t*>(self) + 0x9C);

        if (self->field_00a4 >= self->field_00a0)
        {
            (void)CSF_VTBL(0x0041E5F0);
            S_0041e5f0_GrowArrayCapacity(
                values, nullptr, static_cast<int>(values->field_000c));
        }

        uint32_t index = values->field_0008;
        auto* destination = reinterpret_cast<uint32_t*>(
            values->field_0000 + 4 * index);
        result = index + 1;
        values->field_0008 = result;
        *destination = created;
    }
    return static_cast<int>(result);
}

HOOK(0x00490E70, C_00833908_SetSetupValue, "auto");
