#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// 0x00718C00 — base-setter called by the original; __thiscall(this, int) -> int, ret 4.
CSF_FUNC(int, __fastcall, CGuiControl_Sub718C00, 0x718C00, (void* self, void* /*edx*/, int value));

// int __thiscall sub_721120(int *this, int a2)
// Forwards to the base helper 0x718C00 with the same argument, then stores the
// argument as a raw dword into the CGuiEdit field at +0x14C (declared as a float),
// and returns the base helper's result.
static int __fastcall CGuiEdit_SetField014c(CGuiEdit* self, void* /*edx*/, int value)
{
    int result = CGuiControl_Sub718C00(self, nullptr, value);
    CSF_AT(uint32_t, self, 0x14c) = static_cast<uint32_t>(value);
    return result;
}

HOOK(0x721120, CGuiEdit_SetField014c, "auto");
