#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_ColorChannelScale, 0x0082FEA8);

CSF_FUNC(int, __fastcall, ConfigureEffect, 0x00731A60,
         (void* self, void* edx, int effect, const void* state));
CSF_FUNC(int, __fastcall, StartEffect, 0x00731A80,
         (void* self, void* edx, int effect));

// Capture full EAX: the target tests AL but returns EAX on early exits.
CSF_FUNC(int, __fastcall, QueryEffectStateRegister, 0x00731AB0,
         (void* self, void* edx, int effect));

static int __fastcall CGuiButton_StartHoverEffect(
    CGuiButton* self, void* /*edx*/, char enabled)
{
    int result = static_cast<int>(self->m_pHoverQuad);
    if (result == 0)
        return result;

    // Local stack payload, not a game-object layout. Padding remains untouched.
    struct EffectParameters
    {
        std::uint8_t enabled;
        std::uint8_t padding0[3];
        std::uint32_t reservedWords[3];
        std::uint8_t firstFlag;
        std::uint8_t secondFlag;
        std::uint8_t padding1[2];
        std::uint32_t reservedValue;
        std::uint32_t firstParameter;
        std::uint32_t secondParameter;
        float intensity;
        std::uint32_t trailingValues[8];
    };
    static_assert(sizeof(EffectParameters) == 68);

    EffectParameters state;
    state.trailingValues[0] = 0;
    state.trailingValues[1] = 0;
    state.trailingValues[2] = 0;
    state.trailingValues[3] = 0x3F800000;
    state.trailingValues[4] = 0;
    state.trailingValues[5] = 0;
    state.trailingValues[6] = 0;
    state.trailingValues[7] = 0x3F800000;
    state.reservedWords[1] = 0;
    state.reservedWords[0] = 0;
    state.intensity = 0.0f;
    state.secondParameter = 0;
    state.firstParameter = 0;
    state.enabled = 0;
    state.firstFlag = 0;
    state.secondFlag = 0;
    state.reservedWords[2] = 0;
    state.reservedValue = 0;

    const std::uint8_t hoverActive = static_cast<std::uint8_t>(
        QueryEffectStateRegister(
            reinterpret_cast<void*>(self->m_pFx), nullptr, 1));

    result = QueryEffectStateRegister(
        reinterpret_cast<void*>(self->m_pFx), nullptr, 4);
    if (static_cast<std::uint8_t>(result) != 0 || enabled == 0)
        return result;

    if (hoverActive != 0)
    {
        void* effects = reinterpret_cast<void*>(self->m_pFx);
        const double currentValue =
            reinterpret_cast<double (__fastcall*)(void*, void*)>(
                CSF_AT(void**, effects, 0)[2])(effects, nullptr);
        state.intensity =
            static_cast<float>(currentValue * g_ColorChannelScale);
    }
    else
    {
        state.intensity = 0.0f;
    }

    const std::uint32_t firstParameter = self->field_0120;
    const std::uint32_t secondParameter = self->field_0124;
    state.enabled = static_cast<std::uint8_t>(enabled);
    void* effects = reinterpret_cast<void*>(self->m_pFx);
    state.firstParameter = firstParameter;
    state.secondParameter = secondParameter;

    ConfigureEffect(effects, nullptr, 1, &state);
    return StartEffect(reinterpret_cast<void*>(self->m_pFx), nullptr, 1);
}

HOOK(0x00725A60, CGuiButton_StartHoverEffect, "auto");
