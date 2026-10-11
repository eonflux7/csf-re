#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_ProgressScale, 0x0082E03C);
CSF_GLOBAL(float, g_AudioFadeRate, 0x008639AC);

static char __fastcall S_0070e170_UpdateFadeAndPlayback(
    S_0070e170* self, void* /*edx*/, float elapsed)
{
    const uint32_t fadeState = self->field_0028;

    if (fadeState == 0)
    {
        const float volume = elapsed * g_AudioFadeRate + self->field_0030;
        self->field_0030 = volume;
        if (volume >= self->field_0004)
        {
            self->field_0030 = self->field_0004;
            self->field_0028 = 2;
        }

        void* sound = reinterpret_cast<void*>(self->field_0018);
        const float scaledVolume = self->field_0008 * self->field_0030;
        ((void(__fastcall*)(void*, void*, float))
            CSF_AT(void**, sound, 0)[0x20 / 4])(sound, nullptr, scaledVolume);
    }
    else if (fadeState == 1)
    {
        const float volume = self->field_0030 - elapsed * g_AudioFadeRate;
        self->field_0030 = volume;
        if (volume < g_ProgressScale)
        {
            const uint32_t completionAction = self->field_002c;
            self->field_0030 = 0.0f;
            self->field_0028 = 2;

            if (completionAction == 0)
            {
                ((void(__fastcall*)(S_0070e170*, void*, int))
                    reinterpret_cast<void**>(self->field_0000)[0x10 / 4])(
                        self, nullptr, 0);
            }
            else if (completionAction == 1)
            {
                void* sound = reinterpret_cast<void*>(self->field_0018);
                self->field_003a = 1;
                ((void(__fastcall*)(void*, void*))
                    CSF_AT(void**, sound, 0)[0x10 / 4])(sound, nullptr);
            }
        }

        void* sound = reinterpret_cast<void*>(self->field_0018);
        if (sound)
        {
            const float scaledVolume = self->field_0008 * self->field_0030;
            ((void(__fastcall*)(void*, void*, float))
                CSF_AT(void**, sound, 0)[0x20 / 4])(sound, nullptr, scaledVolume);
        }
    }
    else if (self->field_0044)
    {
        void* sound = reinterpret_cast<void*>(self->field_0018);
        const float scaledVolume = self->field_0008 * self->field_0004;
        self->field_0044 = 0;
        ((void(__fastcall*)(void*, void*, float))
            CSF_AT(void**, sound, 0)[0x20 / 4])(sound, nullptr, scaledVolume);
    }

    char result = static_cast<char>(self->field_0046);
    if (result)
    {
        void* sound = reinterpret_cast<void*>(self->field_0018);
        if (sound)
        {
            result = static_cast<char>(
                ((int(__fastcall*)(void*, void*, uint32_t))
                    CSF_AT(void**, sound, 0)[0x24 / 4])(
                        sound, nullptr, self->field_0010));
            self->field_0046 = 0;
        }
    }
    return result;
}

HOOK(0x0070E3B0, S_0070e170_UpdateFadeAndPlayback, "auto");
