#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_70DDE0, 0x0070DDE0, ());
CSF_FUNC(uint32_t*, __cdecl, Audio_CreateSoundInstance, 0x0070E910, (int a2));
CSF_GLOBAL(float, g_CharacterHeightScale, 0x0082D4BC);

static uint32_t* __fastcall C_00863d80_CreateAndPlaySound(
    C_00863d80* self, void* /*edx*/,
    int playbackArgument, float volume, int soundParameter,
    int instanceIndex, int playbackArgument2, int playbackArgument3,
    char creationFlag)
{
    alignas(8) uint8_t creationBytes[0xC];

    CSF_AT(uint32_t, creationBytes, 0x0) = self->field_0004;
    CSF_AT(int, creationBytes, 0x4) = 0;
    CSF_AT(uint8_t, creationBytes, 0x8) = self->field_01b2;
    CSF_AT(uint8_t, creationBytes, 0x9) = instanceIndex == 1;
    CSF_AT(char, creationBytes, 0xA) = creationFlag;
    CSF_AT(uint8_t, creationBytes, 0xB) = 0;

    if (instanceIndex == 0)
    {
        void* audioManager = reinterpret_cast<void*>(GetGlobalValue_70DDE0());
        int quality = reinterpret_cast<int(__fastcall*)(void*, void*)>(
            CSF_AT(void**, audioManager, 0)[0x6C / 4])(
                audioManager, nullptr);
        CSF_AT(uint8_t, creationBytes, 0xB) = quality < 3;
    }

    if (self->field_01b4)
    {
        CSF_AT(int, creationBytes, 0x4) = 2;
        self->field_01b4 = 0;
    }

    uint32_t* sound = Audio_CreateSoundInstance(
        reinterpret_cast<int>(creationBytes));
    (&self->field_0008)[instanceIndex] = reinterpret_cast<uint32_t>(sound);

    if (!sound)
        return sound;

    if (self->field_01b3)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, int, uint32_t)>(
            CSF_AT(void**, sound, 0)[0x8 / 4])(
                sound, nullptr, 1, 0);
    }

    reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, sound, 0)[0x18 / 4])(
            sound, nullptr, soundParameter);

    if (instanceIndex == 1)
        volume = volume * g_CharacterHeightScale;

    return reinterpret_cast<uint32_t*(__fastcall*)(
        void*, void*, int, float, int, int)>(
            CSF_AT(void**, sound, 0)[0x4 / 4])(
                sound, nullptr, playbackArgument, volume,
                playbackArgument2, playbackArgument3);
}

HOOK(0x00711C10, C_00863d80_CreateAndPlaySound, "auto");
