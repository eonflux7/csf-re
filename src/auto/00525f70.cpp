#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __stdcall, AudioSoundManager_LoadSound, 0x00525EF0, (int a1));

static int __fastcall S_00525f70_SwitchSoundState(
    S_00525f70* self, void* /*edx*/, int state)
{
    int result = static_cast<int>(self->field_0018);
    if (state == result)
        return result;

    uint32_t incomingSound = 0;
    switch (state)
    {
    case 1:
        if (!self->field_0008)
            self->field_0008 = AudioSoundManager_LoadSound(
                static_cast<int>(self->field_001c));
        incomingSound = self->field_0008;
        break;
    case 2:
        if (!self->field_000c)
            self->field_000c = AudioSoundManager_LoadSound(
                static_cast<int>(self->field_0020));
        incomingSound = self->field_000c;
        break;
    case 3:
        if (!self->field_0010)
            self->field_0010 = AudioSoundManager_LoadSound(
                static_cast<int>(self->field_0024));
        incomingSound = self->field_0010;
        break;
    default:
        break;
    }

    if (incomingSound)
    {
        void* sound = reinterpret_cast<void*>(incomingSound);
        ((void (__fastcall*)(void*, void*, int, float))
            CSF_AT(void**, sound, 0)[0x28 / 4])(sound, nullptr, 0, 0.5f);
    }

    const uint32_t previousState = self->field_0018;
    uint32_t outgoingSound = 0;

    result = static_cast<int>(previousState - 1);
    if (previousState == 1)
    {
        outgoingSound = self->field_0008;
    }
    else
    {
        result = static_cast<int>(previousState - 2);
        if (previousState == 2)
        {
            outgoingSound = self->field_000c;
        }
        else
        {
            result = static_cast<int>(previousState - 3);
            if (previousState == 3)
                outgoingSound = self->field_0010;
        }
    }

    if (outgoingSound)
    {
        void* sound = reinterpret_cast<void*>(outgoingSound);
        result = ((int (__fastcall*)(void*, void*))
            CSF_AT(void**, sound, 0)[0x2C / 4])(sound, nullptr);
    }

    self->field_0018 = static_cast<uint32_t>(state);
    return result;
}

HOOK(0x00525F70, S_00525f70_SwitchSoundState, "auto");
