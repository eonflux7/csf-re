#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);

static std::uint32_t __fastcall CSoundInstance_StartPlayback(
    CSoundInstance* self, void* /*edx*/, int startPosition, float fadeDuration)
{
    void* voice = reinterpret_cast<void*>(self->m_voice);
    const double currentVolume =
        reinterpret_cast<double(__fastcall*)(void*, void*)>(
            CSF_AT(void**, voice, 0)[0x50 / 4])(voice, nullptr);

    if (currentVolume != self->m_volume)
    {
        voice = reinterpret_cast<void*>(self->m_voice);
        reinterpret_cast<void(__fastcall*)(void*, void*, float)>(
            CSF_AT(void**, voice, 0)[0x54 / 4])(
                voice, nullptr, self->m_volume);
    }

    self->m_loop = self->field_0038;
    self->field_0064 = fadeDuration;
    self->m_state = 0;
    self->field_0068 = fadeDuration;

    // Original: call [edx+0x3c]; test al, al; jne -> ret 8 with the callee's full eax.
    // Only al is tested, and the whole eax is returned when it is nonzero.
    std::uint32_t result = reinterpret_cast<std::uint32_t(__fastcall*)(void*, void*)>(
        CSF_AT(void**, self, 0)[0x3c / 4])(self, nullptr);
    if ((result & 0xFFu) == 0)
    {
        if (self->field_0064 > g_fZero)
        {
            voice = reinterpret_cast<void*>(self->m_voice);
            self->m_state = 1;
            reinterpret_cast<void(__fastcall*)(void*, void*, float)>(
                CSF_AT(void**, voice, 0)[0x54 / 4])(voice, nullptr, 0.0f);
        }

        voice = reinterpret_cast<void*>(self->m_voice);
        result = reinterpret_cast<std::uint32_t(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, voice, 0)[0x24 / 4])(
                voice, nullptr, startPosition);

        if (startPosition <= 0)
        {
            self->field_004c = self->m_fadeTime;
        }
        else
        {
            const float position = static_cast<float>(startPosition);
            voice = reinterpret_cast<void*>(self->m_voice);
            result = reinterpret_cast<std::uint32_t(__fastcall*)(void*, void*)>(
                CSF_AT(void**, voice, 0)[0x34 / 4])(voice, nullptr);

            const std::int32_t signedLength = static_cast<std::int32_t>(result);
            double length = static_cast<double>(signedLength);
            if (signedLength < 0)
                length += g_UnsignedIntegerBias;

            self->field_004c = static_cast<float>(
                self->m_fadeTime - position / length * self->m_fadeTime);
        }
    }

    return result;
}

HOOK(0x005266C0, CSoundInstance_StartPlayback, "auto");
