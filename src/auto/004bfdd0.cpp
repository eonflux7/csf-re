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

// Callee: __thiscall-style, one stack arg (uint32_t* out), ecx = this.
CSF_FUNC(int, __fastcall, sub_4B1310, 0x004B1310, (void* self, void* edx, uint32_t* a2));

// Callee: __thiscall, 4 stack args (ret 16).
CSF_FUNC(int, __fastcall, sub_4BC870, 0x004BC870, (void* self, void* edx, int a2, int a3, int a4, int a5));

// Original: __thiscall with one stack argument (ret 4), returns 1 via `mov al, 1`.
static char __fastcall S_004bc930_QueryAndDispatchAction(S_004bc930* self, void* /*edx*/, int /*a2*/)
{
    // One 12-byte object: the callee fills all three dwords through the pointer.
    uint32_t result[3] = {0, 0, 0};

    // ecx = this + 0xCC: the sub-object is at +0xCC, its address is passed as `this`.
    sub_4B1310(reinterpret_cast<void*>(&self->field_00cc), nullptr, result);

    // ecx = this + 0x6C: sub-object address, pushed args: 2 and the three dwords.
    sub_4BC870(reinterpret_cast<uint8_t*>(self) + 0x6C, nullptr,
               2,
               static_cast<int>(result[0]),
               static_cast<int>(result[1]),
               static_cast<int>(result[2]));

    return 1;
}

HOOK(0x004BFDD0, S_004bc930_QueryAndDispatchAction, "auto");
