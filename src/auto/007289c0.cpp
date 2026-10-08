#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// 0x00713F20 — base helper invoked by the original; __thiscall(this, int) -> int, ret 4.
CSF_FUNC(int, __fastcall, C_00863f60_Sub713F20, 0x713F20, (void* self, void* /*edx*/, int value));

// int __thiscall sub_7289C0(int *this, int a2)
// Forwards the argument to the base helper 0x713F20, then stores it as a raw dword
// into the C_00865758 field at +0x34 (declared as a float), returning the base
// helper's result unchanged.
static int __fastcall C_00865758_SetField0034(C_00865758* self, void* /*edx*/, int value)
{
    int baseResult = C_00863f60_Sub713F20(self, nullptr, value);
    *reinterpret_cast<uint32_t*>(&self->field_0034) = static_cast<uint32_t>(value);
    return baseResult;
}

HOOK(0x7289C0, C_00865758_SetField0034, "auto");
