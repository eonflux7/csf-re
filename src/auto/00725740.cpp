#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

CSF_FUNC(std::uint32_t*, __fastcall, InitializeState, 0x00725350,
         (std::uint32_t* self, void* edx));
CSF_FUNC(int, __fastcall, CanConfigureEffect, 0x007319C0,
         (void* self, void* edx, int effect));
CSF_FUNC(int, __fastcall, ConfigureEffect, 0x00731A60,
         (void* self, void* edx, int effect, const void* state));
CSF_FUNC(int, __fastcall, StartEffect, 0x00731A80,
         (void* self, void* edx, int effect));

static void __fastcall CGuiButton_StartTextColorEffect(
    CGuiButton* self, void* /*edx*/)
{
    if (self->m_pText == 0)
        return;

    float duration;
    std::memcpy(&duration, &self->field_011c, sizeof(duration));
    if (duration == g_fZero)
        return;

    if (self->m_bEffects == 0)
        return;

    if (CanConfigureEffect(reinterpret_cast<void*>(self->m_pFx),
                           nullptr, 3) == 0)
        return;

    std::uint32_t hoverRed;
    std::memcpy(&hoverRed, &self->m_hoverTextColor, sizeof(hoverRed));
    const std::uint32_t hoverGreen = CSF_AT(std::uint32_t, self, 0x178);
    const std::uint32_t hoverBlue = CSF_AT(std::uint32_t, self, 0x17C);
    const std::uint32_t hoverAlpha = CSF_AT(std::uint32_t, self, 0x180);

    std::uint32_t pressedRed;
    std::memcpy(&pressedRed, &self->m_pressedTextColor, sizeof(pressedRed));
    const std::uint32_t pressedGreen = CSF_AT(std::uint32_t, self, 0x194);
    const std::uint32_t pressedBlue = CSF_AT(std::uint32_t, self, 0x198);
    const std::uint32_t pressedAlpha = CSF_AT(std::uint32_t, self, 0x19C);

    std::uint32_t state[19];
    InitializeState(state, nullptr);
    state[12] = hoverAlpha;
    state[11] = hoverBlue;
    state[15] = pressedBlue;
    state[13] = pressedRed;
    state[14] = pressedGreen;

    void* effects = reinterpret_cast<void*>(self->m_pFx);
    const std::uint32_t durationBits = self->field_011c;
    state[9] = hoverRed;
    state[10] = hoverGreen;
    state[16] = pressedAlpha;
    state[6] = durationBits;

    ConfigureEffect(effects, nullptr, 3, state);
    StartEffect(reinterpret_cast<void*>(self->m_pFx), nullptr, 3);
}

HOOK(0x00725740, CGuiButton_StartTextColorEffect, "auto");
