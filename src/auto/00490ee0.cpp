#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
static CSF_FUNC(int, __cdecl, CreateSetupParameters, 0x004E10D0, (int a1, int a2, int a3, int a4, int a5, int a6));

static int __fastcall C_00833908_SetSetupParameters(
    C_00833908* self, void* /*edx*/,
    int a2, int a3, int a4, int a5, int a6)
{
    void* previous = reinterpret_cast<void*>(self->field_0038);
    if (previous)
    {
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, previous, 0)[0])(previous, nullptr, 1);
        self->field_0038 = 0;
    }

    int result = CreateSetupParameters(
        a2, reinterpret_cast<int>(self), a3, a4, a5, a6);
    const int parameters = result;
    self->field_0038 = result;

    if (result)
    {
        auto* array = reinterpret_cast<S_0041e5f0*>(
            reinterpret_cast<uint8_t*>(self) + 0x9C);
        if (self->field_00a4 >= self->field_00a0)
        {
            (void)CSF_VTBL(0x0041E5F0);
            S_0041e5f0_GrowArrayCapacity(array, nullptr, array->field_000c);
        }

        const uint32_t count = array->field_0008;
        int* destination = reinterpret_cast<int*>(array->field_0000) + count;
        result = static_cast<int>(count + 1);
        array->field_0008 = result;
        *destination = parameters;
    }

    return result;
}

HOOK(0x00490EE0, C_00833908_SetSetupParameters, "auto");
