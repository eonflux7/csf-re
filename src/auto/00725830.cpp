#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_ColorChannelScale, 0x0082FEA8);

CSF_FUNC(int, __fastcall, ColorsMatch, 0x0042D680,
         (float* current, void* edx, float* target));
CSF_FUNC(int, __fastcall, ConfigureEffect, 0x00731A60,
         (void* self, void* edx, int channel, void* parameters));
CSF_FUNC(int, __fastcall, StartEffect, 0x00731A80,
         (void* self, void* edx, int channel));
CSF_FUNC(char, __fastcall, Fx_IsChannelActive, 0x00731AB0,
         (void* self, void* edx, int channel));
CSF_FUNC(int, __fastcall, SetEffectState, 0x00731AD0,
         (void* self, void* edx, int channel));

static int __fastcall CGuiButton_UpdateTextColor(
    CGuiButton* self, void* /*edx*/, char focused)
{
    void* text = reinterpret_cast<void*>(self->m_pText);
    if (!text)
        return 0;

    const float* displayedColor =
        reinterpret_cast<const float* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, text, 0)[0x64 / 4])(text, nullptr);

    std::uint32_t currentColor[4];
    std::memcpy(currentColor, displayedColor, sizeof(currentColor));

    const float* selectedColor;
    if (focused)
    {
        std::uint32_t focusBits;
        std::memcpy(&focusBits, self->m_focusTextColor, sizeof(focusBits));
        selectedColor = focusBits == 0xBF800000u
            ? self->m_hoverTextColor
            : self->m_focusTextColor;
    }
    else
    {
        selectedColor = self->m_idleTextColor;
    }

    std::uint32_t targetColor[4];
    std::memcpy(targetColor, selectedColor, sizeof(targetColor));
    const std::uint32_t targetRed = targetColor[0];
    const std::uint32_t targetGreen = targetColor[1];

    if (Fx_IsChannelActive(
            reinterpret_cast<void*>(self->m_pFx), nullptr, 0))
    {
        void* fx = reinterpret_cast<void*>(self->m_pFx);
        const double opacity =
            reinterpret_cast<double (__fastcall*)(void*, void*)>(
                CSF_AT(void**, fx, 0)[1])(fx, nullptr);
        const float alpha =
            static_cast<float>(opacity * g_ColorChannelScale);
        std::memcpy(&currentColor[3], &alpha, sizeof(alpha));
    }

    if (reinterpret_cast<unsigned char (__fastcall*)(void*, void*)>(
            CSF_AT(void**, self, 0)[0xBC / 4])(self, nullptr))
    {
        std::memcpy(&targetColor[3], &self->field_01dc,
                    sizeof(targetColor[3]));
    }

    if (static_cast<unsigned char>(
            ColorsMatch(reinterpret_cast<float*>(currentColor), nullptr,
                        reinterpret_cast<float*>(targetColor))))
    {
        return SetEffectState(
            reinterpret_cast<void*>(self->m_pFx), nullptr, 2);
    }

    struct TransitionParameters
    {
        std::uint8_t flag;
        std::uint8_t padding0[3];
        std::uint32_t value04;
        std::uint32_t value08;
        std::uint32_t value0C;
        std::uint8_t flag10;
        std::uint8_t flag11;
        std::uint8_t padding12[2];
        std::uint32_t value14;
        std::uint32_t durationBits;
        std::uint32_t value1C;
        std::uint32_t value20;
        std::uint32_t current[4];
        std::uint32_t target[4];
    };

    TransitionParameters parameters;
    std::memcpy(parameters.current, currentColor, sizeof(currentColor));
    parameters.target[0] = targetRed;
    parameters.target[1] = targetGreen;
    parameters.target[2] = targetColor[2];
    parameters.target[3] = targetColor[3];
    parameters.durationBits = self->field_0120;
    parameters.flag = 0;
    parameters.value04 = 0;
    parameters.value08 = 0;
    parameters.value0C = 0;
    parameters.flag10 = 0;
    parameters.flag11 = 0;
    parameters.value14 = 0;
    parameters.value1C = 0;
    parameters.value20 = 0;

    ConfigureEffect(
        reinterpret_cast<void*>(self->m_pFx), nullptr, 2, &parameters);
    return StartEffect(
        reinterpret_cast<void*>(self->m_pFx), nullptr, 2);
}

HOOK(0x00725830, CGuiButton_UpdateTextColor, "auto");
