#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __cdecl, TaskState_Create, 0x0041E240, (int stateType, int param));
CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));

static uint32_t __fastcall C_00833908_SetSetupMode(
    C_00833908* self, void* /*edx*/, int stateType)
{
    void* previousState = CSF_AT(void*, self, 0x30);
    if (previousState)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, previousState, 0)[0])(previousState, nullptr, 1);
        CSF_AT(uint32_t, self, 0x30) = 0;
    }

    uint32_t* state = TaskState_Create(
        stateType, reinterpret_cast<int>(self));
    const uint8_t wasEnabled = CSF_AT(uint8_t, self, 0x174);
    CSF_AT(uint32_t*, self, 0x30) = state;

    uint32_t result = wasEnabled && state ? 1 : 0;
    CSF_AT(uint8_t, self, 0x174) = static_cast<uint8_t>(result);
    if (result)
    {
        auto* states = reinterpret_cast<S_0041e5f0*>(
            reinterpret_cast<uint8_t*>(self) + 0x9C);

        if (states->field_0008 >= states->field_0004)
        {
            (void)CSF_VTBL(0x0041E5F0);
            S_0041e5f0_GrowArrayCapacity(
                states, nullptr, states->field_000c);
        }

        const uint32_t count = states->field_0008;
        auto* destination = reinterpret_cast<uint32_t*>(
            states->field_0000 + 4 * count);
        result = count + 1;
        states->field_0008 = result;
        *destination = reinterpret_cast<uint32_t>(state);
    }
    return result;
}

HOOK(0x00490D70, C_00833908_SetSetupMode, "auto");
