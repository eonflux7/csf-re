#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static hook::Tail __fastcall S_0070e170_ProcessPlaybackRequests(
    void* self, const uint32_t* /*args*/)
{
    auto* sound = static_cast<S_0070e170*>(self);
    void* obj = reinterpret_cast<void*>(sound->field_0018);
    if (!obj)
        return hook::tail_ret(0);

    if (sound->field_0041)
    {
        const bool transitionEnabled = sound->field_0043 != 0;
        sound->field_0041 = 0;

        if (transitionEnabled)
        {
            if (sound->field_0028 != 1)
            {
                const float volume = sound->field_0004;
                sound->field_0028 = 1;
                sound->field_0030 = volume;
            }
            sound->field_002c = 1;
        }
        else if (!sound->field_003a)
        {
            sound->field_003a = 1;
            sound->field_0028 = 2;
            return hook::tail_jump(
                obj, (*reinterpret_cast<void***>(obj))[0x10 / 4]);
        }
        return hook::tail_ret(0);
    }

    if (!CSF_AT(uint8_t, sound, 0x42))
        return hook::tail_ret(0);

    const bool transitionEnabled = sound->field_0043 != 0;
    CSF_AT(uint8_t, sound, 0x42) = 0;

    float volume;
    if (transitionEnabled)
    {
        const uint32_t state = sound->field_0028;
        if (state != 1 && (state != 2 || !sound->field_003a))
        {
            sound->field_003a = 0;
            return hook::tail_ret(0);
        }

        sound->field_0028 = 0;
        sound->field_0030 = 0.0f;
        volume = 0.0f;
    }
    else
    {
        if (!sound->field_003a)
        {
            sound->field_003a = 0;
            return hook::tail_ret(0);
        }

        volume = sound->field_0008 * sound->field_0004;
        sound->field_0028 = 2;
    }

    reinterpret_cast<void (__fastcall*)(void*, void*, float)>(
        CSF_AT(void**, obj, 0)[0x20 / 4])(obj, nullptr, volume);

    obj = reinterpret_cast<void*>(sound->field_0018);
    reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, obj, 0)[0x14 / 4])(obj, nullptr, 1);

    sound->field_003a = 0;
    return hook::tail_ret(0);
}

HOOK_TAIL(0x0070E220, S_0070e170_ProcessPlaybackRequests, "auto");
