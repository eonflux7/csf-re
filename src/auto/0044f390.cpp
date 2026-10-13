#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Forwards this control's m_y value to the alert-icon updater.
CSF_FUNC(int, __fastcall, S_004a0490_UpdateAlertIcons, 0x004A0640, (S_004a0490* self, void* edx));

static int __fastcall CGuiControl_UpdateSceneDisplayTime(CGuiControl* self, void* /*edx*/, int /*a2*/)
{
    return S_004a0490_UpdateAlertIcons(reinterpret_cast<S_004a0490*>(self->m_y), nullptr);
}

HOOK(0x0044F390, CGuiControl_UpdateSceneDisplayTime, "auto");
