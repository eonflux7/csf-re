#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __cdecl, MissionScript_ProcessDirectValue, 0x006913D0, (int* a2, int a3, uint16_t* a4));
CSF_FUNC(char, __cdecl, CoreMemory_ProcessDescribedValue, 0x006914D0, (void* object, uint8_t* state, S_00691860* output, void* description));

static char __cdecl DynamicObjects_SerializeReference(S_00691860* output, bool* state)
{
    int resolvedValue = 0;

    if (CSF_AT(uint8_t, state, 0x14))
    {
        void* owner = CSF_AT(void*, output, 0);
        if (owner)
        {
            void* resolver = static_cast<uint8_t*>(owner) + 4;
            resolvedValue =
                reinterpret_cast<int(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, resolver, 0)[4 / 4])(
                        resolver, nullptr);

            if (!resolvedValue)
            {
                CSF_AT(uint8_t, state, 0) = 1;
                return 0;
            }
        }

        return MissionScript_ProcessDirectValue(
            reinterpret_cast<int*>(resolvedValue),
            reinterpret_cast<int>(state),
            reinterpret_cast<uint16_t*>(output));
    }

    alignas(8) uint8_t descriptionBytes[0x10];

    uint8_t failed = 0;
    if (CSF_AT(uint8_t, state, 0) == 0)
    {
        void* provider = CSF_AT(void*, state, 4);
        uint8_t success =
            reinterpret_cast<uint8_t(__fastcall*)(void*, void*, int*, bool*)>(
                CSF_AT(void**, provider, 0)[0x88 / 4])(
                    provider, nullptr,
                    reinterpret_cast<int*>(descriptionBytes), state);
        failed = success == 0;
    }
    CSF_AT(uint8_t, state, 0) = failed;

    if (!CSF_AT(uint8_t, descriptionBytes, 1) &&
        CSF_AT(uint8_t, state, 0x15))
    {
        void* owner = CSF_AT(void*, output, 0);
        void* resolver = static_cast<uint8_t*>(owner) + 4;
        resolvedValue =
            reinterpret_cast<int(__fastcall*)(void*, void*)>(
                CSF_AT(void**, resolver, 0)[4 / 4])(
                    resolver, nullptr);

        if (!resolvedValue)
        {
            CSF_AT(uint8_t, state, 0) = 1;
            return 0;
        }
    }

    return CoreMemory_ProcessDescribedValue(
        reinterpret_cast<void*>(resolvedValue),
        reinterpret_cast<uint8_t*>(state),
        output,
        descriptionBytes);
}

HOOK(0x00442E70, DynamicObjects_SerializeReference, "auto");
