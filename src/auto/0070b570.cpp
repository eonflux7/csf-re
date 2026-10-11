#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
// The packet's bind line is `()`, but the original pushes two cdecl arguments before this call.
// The argument-taking pointer type below calls the same address with those two pushes.
using Constant1e8Fn = double(__cdecl*)(uint32_t, uint32_t);

// Original: push esi; esi = this; call GetGlobalValue; push [eax+0xF0]; push [esi+4]; call Constant1e8;
// add esp,8; pop esi; ret. The callee's st0 (double) is the return value, passed through unchanged.
static double __fastcall GetWaterConstant1e8(void* self, void* /*edx*/)
{
    uint8_t* globalValue = reinterpret_cast<uint8_t*>(GetGlobalValue_6C5760());
    uint32_t firstArg = CSF_AT(uint32_t, self, 0x04);
    uint32_t secondArg = CSF_AT(uint32_t, globalValue, 0xF0);
    Constant1e8Fn constant1e8 = reinterpret_cast<Constant1e8Fn>(CSF_ADDR_FN(0x006C4E50));
    return constant1e8(firstArg, secondArg);
}

HOOK(0x0070B570, GetWaterConstant1e8, "auto");
