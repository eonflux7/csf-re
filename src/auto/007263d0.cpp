#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CanSetEffectState, 0x007319C0,
         (void* self, void* edx, int state));
CSF_FUNC(int, __fastcall, SetEffectState, 0x00731AD0,
         (void* self, void* edx, int state));
CSF_FUNC(void, __fastcall, ResetButtonEffects, 0x00726090,
         (CGuiButton* self, void* edx, char enabled));

static char __fastcall CGuiButton_ActivateTextEffect(
    CGuiButton* self, void* /*edx*/)
{
    if (self->m_pText == 0)
        return 0;

    const std::uint8_t effectsEnabled = self->m_bEffects;
    if (effectsEnabled == 0)
        return 0;

    if (CanSetEffectState(
            reinterpret_cast<void*>(self->m_pFx), nullptr, 4) == 0)
        return 0;

    void* effect = reinterpret_cast<void*>(self->m_pFx);
    const unsigned char needsReset =
        reinterpret_cast<unsigned char (__fastcall*)(void*, void*)>(
            CSF_AT(void**, effect, 0)[0x18 / 4])(effect, nullptr);

    if (needsReset != 0)
        ResetButtonEffects(self, nullptr, 1);

    return static_cast<char>(SetEffectState(
        reinterpret_cast<void*>(self->m_pFx), nullptr, 4));
}

HOOK(0x007263D0, CGuiButton_ActivateTextEffect, "auto");
