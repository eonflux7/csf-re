#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __cdecl, MissionScript_ProcessDirectValue, 0x006913D0,
         (int* a2, int a3, uint16_t* a4));
CSF_FUNC(char, __cdecl, MissionScript_ProcessDescribedValue, 0x006914D0,
         (int a1, uint8_t* a2, void* a3, int a4));

static int DynamicObjects_ResolveOwnerValue(void* owner)
{
    void* resolver = static_cast<uint8_t*>(owner) + 0x04;
    return (reinterpret_cast<int(__fastcall*)(void*, void*)>(
        CSF_AT(void**, resolver, 0)[0x04 / 4]))(resolver, nullptr);
}

static char __cdecl DynamicObjects_SerializeReference(
    S_00691860* context, bool* state)
{
    int resolvedValue = 0;

    if (state[20])
    {
        void* owner = CSF_AT(void*, context, 0);
        if (owner)
        {
            resolvedValue = DynamicObjects_ResolveOwnerValue(owner);
            if (!resolvedValue)
            {
                *state = true;
                return 0;
            }
        }

        return MissionScript_ProcessDirectValue(
            reinterpret_cast<int*>(resolvedValue),
            reinterpret_cast<int>(state),
            reinterpret_cast<uint16_t*>(context));
    }

    alignas(8) uint8_t descriptorBytes[0x10];
    int* descriptor = reinterpret_cast<int*>(descriptorBytes);

    bool failed = false;
    if (!*state)
    {
        void* provider = CSF_AT(void*, state, 0x04);
        failed =
            !(reinterpret_cast<bool(__fastcall*)(void*, void*, int*, bool*)>(
                CSF_AT(void**, provider, 0)[0x88 / 4]))(
                    provider, nullptr, descriptor, state);
    }
    *state = failed;

    if (!CSF_AT(uint8_t, descriptorBytes, 0x01) && state[21])
    {
        resolvedValue =
            DynamicObjects_ResolveOwnerValue(CSF_AT(void*, context, 0));
        if (!resolvedValue)
        {
            *state = true;
            return 0;
        }
    }

    return MissionScript_ProcessDescribedValue(
        resolvedValue,
        reinterpret_cast<uint8_t*>(state),
        context,
        reinterpret_cast<int>(descriptor));
}

HOOK(0x00442E70, DynamicObjects_SerializeReference, "auto");
