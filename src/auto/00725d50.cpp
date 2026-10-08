#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(int, __fastcall, CGuiControl_ResetInput, 0x00717F70,
         (void* self, void* edx));
CSF_FUNC(void, __fastcall, CGuiButton_ApplyPendingQuadTimes, 0x00725A00,
         (CGuiButton* self, void* edx));
CSF_FUNC(int, __fastcall, CGuiControl_ForwardEventToFirstEffect, 0x00718C00,
         (void* self, void* edx, int event));
CSF_FUNC(int, __fastcall, SetEffectIndex, 0x00731960,
         (void* self, void* edx, int index));

static int __fastcall CGuiButton_ResetStateAndEffects(
    CGuiButton* self, void* /*edx*/)
{
    self->m_state = 0;
    CGuiControl_ResetInput(self, nullptr);

    if (self->m_bStateTimes) {
        CGuiButton_ApplyPendingQuadTimes(self, nullptr);
        self->m_bStateTimes = 0;
    }

    self->field_01d0 = 1.0f;
    CGuiControl_ForwardEventToFirstEffect(self, nullptr, 0x3F800000);
    return SetEffectIndex(
        reinterpret_cast<void*>(self->m_pFx), nullptr, -1);
}

HOOK(0x00725D50, CGuiButton_ResetStateAndEffects, "auto");
