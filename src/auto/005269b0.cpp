#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

// Stores the sound resource on the instance, reads its duration (vtbl slot 0x70),
// wraps the stored play position modulo that duration when it is positive,
// then, if the position has passed the duration, restarts through the resource
// (slots 0x6C, 0x28) and, when the stored pair is not (1.0f, 1.0f), applies it (slot 0x68).
static void __fastcall SoundInstance_BindSampleAndWrapOffset(void* self, void* /*edx*/, void* sample)
{
    CSF_AT(void*, self, 0x60) = sample;
    if (!sample)
        return;

    float duration = ((float(__fastcall*)(void*, void*))(CSF_AT(void**, sample, 0)[0x70 / 4]))(sample, nullptr);
    CSF_AT(float, self, 0x4C) = duration;

    if (duration <= CSF_AT(float, self, 0x48) && duration > g_fZero)
    {
        float remaining = CSF_AT(float, self, 0x48);
        int wholeCycles = static_cast<int>(remaining / duration);
        CSF_AT(float, self, 0x48) = remaining - static_cast<float>(wholeCycles) * duration;
    }

    if (duration > CSF_AT(float, self, 0x48))
    {
        void* target = CSF_AT(void*, self, 0x60);
        float position = CSF_AT(float, self, 0x48);
        int handle = ((int(__fastcall*)(void*, void*, float))(CSF_AT(void**, target, 0)[0x6C / 4]))(target, nullptr, position);

        target = CSF_AT(void*, self, 0x60);
        ((void(__fastcall*)(void*, void*, int, int))(CSF_AT(void**, target, 0)[0x28 / 4]))(target, nullptr, handle, 0);

        if (CSF_AT(int32_t, self, 0x58) != 0x3F800000 || CSF_AT(int32_t, self, 0x5C) != 0x3F800000)
        {
            target = CSF_AT(void*, self, 0x60);
            ((void(__fastcall*)(void*, void*, int32_t, int32_t))(CSF_AT(void**, target, 0)[0x68 / 4]))(
                target, nullptr, CSF_AT(int32_t, self, 0x58), CSF_AT(int32_t, self, 0x5C));
        }
    }
}

HOOK(0x005269B0, SoundInstance_BindSampleAndWrapOffset, "auto");
