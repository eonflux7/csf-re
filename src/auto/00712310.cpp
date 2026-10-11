#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, sub_7128F0, 0x007128F0, (int a1, unsigned int a2));
CSF_FUNC(void, __cdecl, sub_7DF700, 0x007DF700, (int a1, int a2));
CSF_FUNC(uint32_t*, __cdecl, sub_7E1380, 0x007E1380, (uint32_t* a1, int a2, int a3, ...));

using AudioEffectEvaluator = int (__cdecl*)(int, unsigned int);

CSF_GLOBAL(float, g_AudioEffectScale, 0x008A85D8);
CSF_GLOBAL(float, g_AudioEffectFactor, 0x008A85DC);
CSF_GLOBAL(AudioEffectEvaluator, g_AudioEffectEvaluator, 0x008A85E0);

static int __fastcall C_00863dd0_UpdateAudioEffect(
    C_00863dd0* self, void* /*edx*/, int parameter)
{
    void* effect = reinterpret_cast<void*>(self->field_0004);
    const int evaluatorContext = CSF_AT(int, effect, 0x50);
    if (!evaluatorContext)
        return static_cast<int>(self->field_0004);

    CSF_AT(float, effect, 0x40) =
        g_AudioEffectFactor * self->field_0008 * g_AudioEffectScale;

    effect = reinterpret_cast<void*>(self->field_0004);
    if (CSF_AT(uint32_t*, effect, 0x30))
    {
        const float scaledValue =
            g_AudioEffectFactor * self->field_0008 * g_AudioEffectScale;

        // This variadic interface takes a four-byte float payload, not a double.
        uint32_t payload;
        std::memcpy(&payload, &scaledValue, sizeof(payload));
        sub_7E1380(CSF_AT(uint32_t*, effect, 0x30), 5, 1, payload);
    }

    const int evaluatedValue =
        g_AudioEffectEvaluator(evaluatorContext, static_cast<unsigned int>(parameter));
    CSF_AT(int, reinterpret_cast<void*>(self->field_0004), 0x48) =
        evaluatedValue;

    if (CSF_AT(uint32_t*, reinterpret_cast<void*>(self->field_0004), 0x30))
    {
        const int value =
            g_AudioEffectEvaluator(evaluatorContext, static_cast<unsigned int>(parameter));
        sub_7E1380(
            CSF_AT(uint32_t*, reinterpret_cast<void*>(self->field_0004), 0x30),
            7, 1, value);
    }

    sub_7DF700(static_cast<int>(self->field_0004), 1);
    return 0;
}

HOOK(0x00712310, C_00863dd0_UpdateAudioEffect, "auto");
