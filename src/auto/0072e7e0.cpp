#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// char __thiscall sub_72C870(_DWORD *this, int a2)  rets=[4]
CSF_FUNC(char, __fastcall, CGuiRadioGroup_ApplyValue_0x72C870, 0x72C870,
         (CGuiRadioGroup* self, void* /*edx*/, int a2));

// Forward a non-zero value to the group's value handler; a zero value is
// returned unchanged (the original short-circuits without a call).
static char __fastcall CGuiRadioGroup_SetValueIfNonZero(CGuiRadioGroup* self, void* /*edx*/, int a2)
{
    if (a2)
        return CGuiRadioGroup_ApplyValue_0x72C870(self, nullptr, a2);
    return static_cast<char>(a2);
}

HOOK(0x72E7E0, CGuiRadioGroup_SetValueIfNonZero, "auto");
