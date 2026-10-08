#include <cstdint>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_TextEffectEpsilon, 0x00865324);

CSF_FUNC(std::uint32_t*, __fastcall, InitializeState, 0x00725350,
         (std::uint32_t* self, void* edx));
CSF_FUNC(int, __fastcall, CanConfigureEffect, 0x007319C0,
         (void* self, void* edx, int effect));
CSF_FUNC(char, __fastcall, QueryEffectState, 0x00731AB0,
         (void* self, void* edx, int effect));
CSF_FUNC(int, __fastcall, ConfigureEffect, 0x00731A60,
         (void* self, void* edx, int effect, const void* state));
CSF_FUNC(int, __fastcall, StartEffect, 0x00731A80,
         (void* self, void* edx, int effect));

static void __fastcall CGuiButton_StartTextFadeEffect(
    CGuiButton* self, void* /*edx*/, float duration, char mode)
{
    if (self->m_pText == 0)
        return;

    // Unordered comparisons proceed, as in the original x87 test.
    if (std::fabs(-duration) <= g_TextEffectEpsilon)
        return;

    if (self->m_bEffects == 0)
        return;

    if (CanConfigureEffect(reinterpret_cast<void*>(self->m_pFx),
                           nullptr, 4) == 0)
        return;

    struct EffectStateStorage
    {
        std::uint32_t initialWords[3];
        std::uint32_t firstParameter;
        std::uint8_t mode;
        std::uint8_t previousState;
        std::uint8_t padding[2];
        std::uint32_t secondParameter;
        float duration;
        std::uint32_t remainingWords[10];
    };

    static_assert(sizeof(EffectStateStorage) == 68);
    EffectStateStorage state;
    InitializeState(reinterpret_cast<std::uint32_t*>(&state), nullptr);

    if (static_cast<std::uint8_t>(QueryEffectState(
            reinterpret_cast<void*>(self->m_pFx), nullptr, 2)) != 0)
    {
        std::uint32_t color[4];
        void* effects = reinterpret_cast<void*>(self->m_pFx);
        reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, effects, 0)[3])(effects, nullptr, color);

        void* text = reinterpret_cast<void*>(self->m_pText);
        color[3] = 0x437F0000;
        reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, text, 0)[24])(text, nullptr, color);
    }

    const std::uint32_t firstParameter = self->field_01e0;
    const std::uint32_t secondParameter = self->field_01e4;
    std::memcpy(&state.duration, &duration, sizeof(duration));
    state.firstParameter = firstParameter;

    void* effects = reinterpret_cast<void*>(self->m_pFx);
    state.secondParameter = secondParameter;
    state.mode = static_cast<std::uint8_t>(mode);
    const char previousState = QueryEffectState(effects, nullptr, 4);

    effects = reinterpret_cast<void*>(self->m_pFx);
    state.previousState = static_cast<std::uint8_t>(previousState);
    ConfigureEffect(effects, nullptr, 4, &state);
    StartEffect(reinterpret_cast<void*>(self->m_pFx), nullptr, 4);
}

HOOK(0x00725C30, CGuiButton_StartTextFadeEffect, "auto");
