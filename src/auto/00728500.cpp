#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// 0x00713F20 — base helper called by the original; __thiscall(this, int) -> int, ret 4.
CSF_FUNC(int, __fastcall, C_00863f60_Sub713F20, 0x713F20, (void* self, void* /*edx*/, int value));

// int __thiscall sub_728500(int *this, int a2)
// Forwards the argument to the base helper 0x713F20, then stores the same raw
// dword argument into the field at +0x50 (declared float), and returns the
// base helper's result.
static int __fastcall C_008657e0_SetField0050(C_008657e0* self, void* /*edx*/, int value)
{
    int result = C_00863f60_Sub713F20(self, nullptr, value);
    // The original re-reads the argument from the stack after the call.
    CSF_AT(uint32_t, self, 0x50) = static_cast<uint32_t>(value);
    return result;
}

HOOK(0x728500, C_008657e0_SetField0050, "auto");

