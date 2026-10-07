#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// 0x00713F20 — base-setter called by the original; __thiscall(this, int) -> int, ret 4.
CSF_FUNC(int, __fastcall, C_00865a10_BaseSub713F20, 0x713F20, (void* self, void* /*edx*/, int value));

// int __thiscall sub_728AD0(int *this, int a2)
// Forwards to the base helper 0x713F20 with the same argument, then stores the
// argument as a raw dword into the field at +0x44 (declared as a float), and
// returns the base helper's result.
static int __fastcall C_00865a10_SetRawField0044(C_00865a10* self, void* /*edx*/, int value)
{
    int result = C_00865a10_BaseSub713F20(self, nullptr, value);
    CSF_AT(uint32_t, self, 0x44) = static_cast<uint32_t>(value);
    return result;
}

HOOK(0x728AD0, C_00865a10_SetRawField0044, "auto");
