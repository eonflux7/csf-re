#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns nonzero when the control is selectable and its type id (vtable slot +0x60)
// is not 10. Reads CGuiControl::m_bSelectable at +0x115 first (short-circuit).
static int __fastcall CGuiSlider_IsSelectableAndNotType10(CGuiSlider* self, void* /*edx*/)
{
    if (!self->m_bSelectable)
        return 0;

    void** vtbl = *reinterpret_cast<void***>(self);
    int typeId = ((int(__fastcall*)(void*, void*))vtbl[0x60 / 4])(self, nullptr);

    return typeId != 10;
}

HOOK(0x718700, CGuiSlider_IsSelectableAndNotType10, "auto");
