#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// Sets the "disabled" flag on a GUI control and notifies it through vtable slot +0x214.
static int __fastcall CGuiControl_SetDisabled(CGuiControl* self, void* /*edx*/, char disabled)
{
    self->m_bDisabled = disabled;
    return ((int(__fastcall*)(void*, void*))self->vtbl[0x214 / 4])(self, nullptr);
}

HOOK(0x00718080, CGuiControl_SetDisabled, "auto");
