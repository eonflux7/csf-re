#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// 0x0072A3A0 (ret 4)
// Sets the panel's initial-focus child from an id: looks the child up via vtable slot +0xFC
// and stores the resulting control pointer in +0x154 (m_pInitialFocus). Null id is ignored
// (field untouched), and the argument/id itself is returned in that case.
static int __fastcall CGuiPanel_SetInitialFocusById(CGuiPanel* self, void* /*edx*/, int a2)
{
    int result = a2;
    if (a2)
    {
        void** vtbl = *reinterpret_cast<void***>(self);
        result = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(vtbl[0xFC / 4])(self, nullptr, a2);
        self->m_pInitialFocus = result;
    }
    return result;
}

HOOK(0x0072A3A0, CGuiPanel_SetInitialFocusById, "auto");
