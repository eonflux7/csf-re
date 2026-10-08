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

// Forwarded callee at 0x7A36E0 (__stdcall, pops 4 bytes / 1 arg).
CSF_FUNC(int, __fastcall, Sub_7A36E0, 0x7A36E0, (void* self, void* edx, int a1));

// Thin __stdcall thunk at 0x750C50: tail-jumps to 0x7A36E0 passing its single argument through.
static int __fastcall ForwardTo7A36E0(void* self, void* edx, int a1)
{
    // the original tail-jumps with ecx/edx untouched: the callee takes `this` in ecx
    return Sub_7A36E0(self, edx, a1);
}

HOOK(0x750C50, ForwardTo7A36E0, "auto");
