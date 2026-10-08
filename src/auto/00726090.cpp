#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CGuiButton_StartHoverEffect, 0x00725A60,
         (CGuiButton* self, void* edx, char enabled));
CSF_FUNC(int, __fastcall, CGuiButton_UpdateTextColor, 0x00725830,
         (CGuiButton* self, void* edx, char enabled));
CSF_FUNC(int, __fastcall, SetEffectState, 0x00731AD0,
         (void* self, void* edx, int state));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static void __fastcall CGuiButton_ResetEffects(
    CGuiButton* self, void* /*edx*/, char enabled)
{
    float activationValue;
    std::memcpy(&activationValue, &self->field_0120, sizeof(activationValue));

    if (activationValue != g_fZero && self->m_bEffects != 0)
    {
        CGuiButton_StartHoverEffect(self, nullptr, enabled);
        CGuiButton_UpdateTextColor(self, nullptr, enabled);

        reinterpret_cast<void (__fastcall*)(void*, void*)>(
            CSF_AT(void**, self, 0)[0x1E0 / 4])(self, nullptr);

        SetEffectState(reinterpret_cast<void*>(self->m_pFx), nullptr, 0);
    }
}

HOOK(0x00726090, CGuiButton_ResetEffects, "auto");
