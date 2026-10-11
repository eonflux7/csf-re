#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(int, __cdecl, CreateSetupFloatParameters, 0x004E56E0,
         (int a1, int a2, float a3, int a4, int a5));

static int __fastcall C_00833908_SetSetupFloatParameters(
    C_00833908* self, void* /*edx*/,
    int setup, float value, int optionA, int optionB)
{
    void* previous = reinterpret_cast<void*>(self->field_003c);
    if (previous)
    {
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, previous, 0)[0])(previous, nullptr, 1);
        self->field_003c = 0;
    }

    int result = CreateSetupFloatParameters(
        setup, reinterpret_cast<int>(self), value, optionA, optionB);
    const int parameters = result;
    self->field_003c = result;

    if (parameters)
    {
        auto* entries = reinterpret_cast<S_0041e5f0*>(
            reinterpret_cast<std::uint8_t*>(self) + 0x9C);

        if (self->field_00a4 >= self->field_00a0)
        {
            (void)CSF_VTBL(0x0041E5F0);
            S_0041e5f0_GrowArrayCapacity(
                entries, nullptr, entries->field_000c);
        }

        const std::uint32_t index = entries->field_0008;
        int* destination =
            reinterpret_cast<int*>(entries->field_0000) + index;
        result = static_cast<int>(index + 1);
        entries->field_0008 = result;
        *destination = parameters;
    }

    return result;
}

HOOK(0x00490F60, C_00833908_SetSetupFloatParameters, "auto");
