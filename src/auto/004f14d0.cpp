#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Callee at 0x4F13B0: __thiscall, 3 stack args (level, request address, out pointer), ret 12.
CSF_FUNC(bool, __fastcall, S004f0cd0_TryFallbackLevel, 0x004F13B0,
         (void* self, void* edx, int a2, int a3, uint32_t* a4));

// Original: 0x4F14D0, __thiscall(this, int requestAddr, uint32_t* out), ret 8.
// Returns true if the request's field at +8 is non-negative and any of the
// fallback levels 2, 1, 0 succeeds (tried in that order, short-circuiting).
static bool __fastcall S004f0cd0_TestFallbackQuery(S_004f0cd0* self, void* /*edx*/,
                                                   int requestAddr, uint32_t* outPtr)
{
    int* request = reinterpret_cast<int*>(static_cast<uintptr_t>(requestAddr));
    int requestField8 = *reinterpret_cast<int*>(static_cast<uintptr_t>(requestAddr) + 8);
    if (requestField8 < 0)
        return false;

    if (S004f0cd0_TryFallbackLevel(self, nullptr, 2, requestAddr, outPtr))
        return true;
    if (S004f0cd0_TryFallbackLevel(self, nullptr, 1, requestAddr, outPtr))
        return true;
    if (S004f0cd0_TryFallbackLevel(self, nullptr, 0, requestAddr, outPtr))
        return true;
    return false;
}

HOOK(0x004F14D0, S004f0cd0_TestFallbackQuery, "auto");
