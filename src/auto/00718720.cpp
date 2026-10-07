#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// Reads the pointer to the first effect in this control's effect list (offset +0x128),
// clears the field, and returns the detached head pointer to the caller.
static int __fastcall CGuiControl_DetachEffects(CGuiControl* self, void* /*edx*/)
{
    int detachedEffects = static_cast<int>(self->m_pEffects);
    self->m_pEffects = 0;
    return detachedEffects;
}

HOOK(0x718720, CGuiControl_DetachEffects, "auto");
