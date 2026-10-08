#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(int, __fastcall, ResetControlInput, 0x00717F70,
         (void* self, void* edx));
CSF_FUNC(void, __fastcall, ApplyPendingQuadTimes, 0x00725A00,
         (CGuiButton* self, void* edx));
CSF_FUNC(int, __fastcall, ForwardEventToFirstEffect, 0x00718C00,
         (void* self, void* edx, int event));
CSF_FUNC(int, __fastcall, SetEffectIndex, 0x00731960,
         (void* self, void* edx, int index));

static int __fastcall CGuiButton_ResetStateAndEffects(
    CGuiButton* self, void* /*edx*/)
{
    self->field_0148 = 0;
    ResetControlInput(self, nullptr);

    if (self->m_bStateTimes) {
        ApplyPendingQuadTimes(self, nullptr);
        self->m_bStateTimes = 0;
    }

    self->field_01d0 = 1.0f;
    ForwardEventToFirstEffect(self, nullptr, 0x3F800000);
    return SetEffectIndex(
        reinterpret_cast<void*>(self->m_pFx), nullptr, -1);
}

HOOK(0x00725D50, CGuiButton_ResetStateAndEffects, "auto");
