#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Forwards a non-zero index/selection value to the virtual setter at vtable
// offset +0x158 (slot 86). Zero is passed through unchanged without any call.
static int __fastcall CGuiRadioGroup_SetActiveIndex(CGuiRadioGroup* self, void* /*edx*/, int value)
{
    if (value)
    {
        void** vtbl = CSF_AT(void**, self, 0);
        int (__fastcall* setter)(void*, void*, int) =
            reinterpret_cast<int (__fastcall*)(void*, void*, int)>(vtbl[0x158 / 4]);
        return setter(self, nullptr, value);
    }
    return value;
}

HOOK(0x72E800, CGuiRadioGroup_SetActiveIndex, "auto");
