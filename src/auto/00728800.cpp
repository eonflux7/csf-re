#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// 0x00713F20 — base setter the original forwards to; __thiscall(this, int) -> int, ret 4.
CSF_FUNC(int, __fastcall, CGuiControl_Sub713F20, 0x713F20, (void* self, void* /*edx*/, int value));

// int __thiscall sub_728800(int *this, int a2)
// Forwards to the base helper 0x713F20 with the same argument, then stores the
// argument as a raw dword into the field at +0x38 (declared as a float), and
// returns the base helper's result.
static int __fastcall CGuiEdit_SetField0038(C_008658f8* self, void* /*edx*/, int value)
{
    int result = CGuiControl_Sub713F20(self, nullptr, value);
    CSF_AT(uint32_t, self, 0x38) = static_cast<uint32_t>(value);
    return result;
}

HOOK(0x728800, CGuiEdit_SetField0038, "auto");
