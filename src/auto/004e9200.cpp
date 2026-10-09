#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __cdecl, MissionScript_ProcessDirectValue, 0x006913D0,
         (int* a2, int a3, uint16_t* a4));
CSF_FUNC(char, __cdecl, MissionScript_ProcessDescribedValue, 0x006914D0,
         (int a1, uint8_t* a2, void* a3, int a4));

static char __cdecl MissionScript_ResolveAndProcessValue(S_00691860* context, bool* state)
{
    int resolvedValue = 0;

    if (state[20])
    {
        void* owner = CSF_AT(void*, context, 0);
        if (owner)
        {
            void* resolver = static_cast<uint8_t*>(owner) + 0x18;
            resolvedValue =
                (reinterpret_cast<int(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, resolver, 0)[1]))(resolver, nullptr);

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

    int descriptor[4];
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

    if (!CSF_AT(uint8_t, descriptor, 0x01) && state[21])
    {
        void* owner = CSF_AT(void*, context, 0);
        void* resolver = static_cast<uint8_t*>(owner) + 0x18;
        resolvedValue =
            (reinterpret_cast<int(__fastcall*)(void*, void*)>(
                CSF_AT(void**, resolver, 0)[1]))(resolver, nullptr);

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

HOOK(0x004E9200, MissionScript_ResolveAndProcessValue, "auto");
