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

// 0x006687B0 - library callee reached by the tail jump. No stack arguments
// (its `ret` immediate is 0), so it is a plain __thiscall taking only `this`.
CSF_FUNC(int, __fastcall, Sub_6687B0, 0x6687B0, (void* self, void* edx));

// 0x0073D4C0
// Tail-call thunk: adjusts `this` to the sub-object embedded at +0x9C
// (0x9c = 156) and jumps straight into sub_6687B0 with no extra arguments.
// The original emits `add ecx, 0x9c` / `jmp 0x6687b0`, so the returned value
// is exactly whatever the callee returns.
static int __fastcall ForwardToSub6687B0AtOffset9c(void* self, void* /*edx*/)
{
    return Sub_6687B0(static_cast<uint8_t*>(self) + 0x9C, nullptr);
}

HOOK(0x73D4C0, ForwardToSub6687B0AtOffset9c, "auto");
