#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Forwards an event value to the first effect in the control's effect list.
// If the control has no effects, the value is returned unchanged.
static int __fastcall CGuiControl_ForwardToFirstEffect(CGuiControl* self, void* /*edx*/, int a2)
{
    uint32_t effects = self->m_pEffects;
    if (effects)
    {
        void** vtbl = CSF_AT(void**, effects, 0);
        ((void(__fastcall*)(void*, void*, int))vtbl[0x64 / 4])(
            reinterpret_cast<void*>(effects), nullptr, a2);
    }
    return a2;
}

HOOK(0x7188E0, CGuiControl_ForwardToFirstEffect, "auto");
