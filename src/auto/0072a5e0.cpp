#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Calls CGuiPanel's virtual method at vtable slot +0x23C (index 0x8F) with argument 0,
// then clears the float state field at +0x178. Returns the callee's result in eax.
static int __fastcall CGuiPanel_ResetAnimState(CGuiPanel* self, void* /*edx*/)
{
    void** vtbl = *reinterpret_cast<void***>(self);
    int result = (reinterpret_cast<int(__fastcall*)(void*, void*, int)>(vtbl[0x8F]))(self, nullptr, 0);
    self->field_0178 = 0.0f;
    return result;
}

HOOK(0x72A5E0, CGuiPanel_ResetAnimState, "auto");
