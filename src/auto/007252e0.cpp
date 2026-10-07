#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, CGuiControl_Update, 0x719530,
         (CGuiControl* self, void* edx, float deltaTime));

static char __fastcall CGuiButton_Update(
    CGuiButton* self, void* /*edx*/, float deltaTime)
{
    void* effects = reinterpret_cast<void*>(self->m_pFx);
    if (effects)
    {
        auto updateEffects = reinterpret_cast<void(__fastcall*)(void*, void*, float)>(
            CSF_AT(void**, effects, 0)[0]);
        updateEffects(effects, nullptr, deltaTime);
    }

    return CGuiControl_Update(self, nullptr, deltaTime);
}

HOOK(0x7252E0, CGuiButton_Update, "auto");
