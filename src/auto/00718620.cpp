#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Set the focused flag. When turning focus ON, notify the parent control through
// its virtual slot +0x230, passing this control as the argument. Returns this
// when no notification was made, otherwise forwards the callee's return value.
static int __fastcall CGuiControl_SetFocused(CGuiControl* self, void* /*edx*/, char bFocused)
{
    self->m_bFocused = static_cast<uint8_t>(bFocused);
    if (bFocused)
    {
        void* pParent = reinterpret_cast<void*>(self->m_pParent);
        if (pParent)
        {
            void** vtbl = *reinterpret_cast<void***>(pParent);
            return ((int (__fastcall*)(void*, void*, CGuiControl*))vtbl[0x230 / 4])(pParent, nullptr, self);
        }
    }
    return static_cast<int>(reinterpret_cast<uintptr_t>(self));
}

HOOK(0x718620, CGuiControl_SetFocused, "auto");
