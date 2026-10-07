#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Forwards (a2, a3) to the control's effects object (slot +0x50 of its vtable).
// If there are no effects, returns 0 and does nothing. A null name is replaced
// by the constant empty string at 0x0082E65C.
static int __fastcall CGuiControl_ForwardToEffects(CGuiControl* self, void* /*edx*/, int a2, const char* a3)
{
    uint32_t effects = self->m_pEffects;
    if (effects == 0)
        return 0;

    const char* name = a3;
    if (name == nullptr)
        name = CSF_STR(0x0082E65C, "");

    void* effectsObj = reinterpret_cast<void*>(effects);
    void** vtbl = *reinterpret_cast<void***>(effectsObj);
    int(__fastcall* method)(void*, void*, int, const char*) =
        reinterpret_cast<int(__fastcall*)(void*, void*, int, const char*)>(vtbl[0x50 / 4]);
    return method(effectsObj, nullptr, a2, name);
}

HOOK(0x00718B00, CGuiControl_ForwardToEffects, "auto");
