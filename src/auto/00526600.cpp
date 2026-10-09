#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

static char __fastcall CSoundInstance_SetVoiceRateAndVolume(
    CSoundInstance* self, void* /*edx*/, float rate, float attenuation)
{
    char result = static_cast<char>(self->field_0039);
    if (result)
    {
        void* voice = reinterpret_cast<void*>(self->m_voice);
        if (rate == g_TransitionOne)
        {
            reinterpret_cast<void(__fastcall*)(void*, void*)>(
                CSF_AT(void**, voice, 0)[0x64 / 4])(voice, nullptr);
        }
        else
        {
            reinterpret_cast<void(__fastcall*)(void*, void*, float)>(
                CSF_AT(void**, voice, 0)[0x60 / 4])(voice, nullptr, rate);
        }

        voice = reinterpret_cast<void*>(self->m_voice);
        float volume = attenuation * self->m_volume;
        result = reinterpret_cast<char(__fastcall*)(void*, void*, float)>(
            CSF_AT(void**, voice, 0)[0x54 / 4])(voice, nullptr, volume);
    }
    return result;
}

HOOK(0x00526600, CSoundInstance_SetVoiceRateAndVolume, "auto");
