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
// Queries three values into a local 0xC-byte buffer through the sub-object at +0xCC,
// then dispatches action 22 through the sub-object at +0x6C with those values.
static char __fastcall S_004bc930_DispatchActionCode22(S_004bc930* self, void* /*edx*/, int /*a2*/)
{
    // Local frame buffer of 0xC bytes: a3 at [ebp-0xC], a4 at [ebp-8], a5 at [ebp-4].
    uint32_t queryResults[3] = {0, 0, 0};

    // ecx = this + 0xCC: sub-object address is passed as `this`; &queryResults is the single stack arg.
    sub_4B1310(reinterpret_cast<void*>(&self->field_00cc), nullptr, &queryResults[0]);

    // ecx = this + 0x6C: sub-object address; pushed args are 22, a3, a4, a5.
    sub_4BC870(reinterpret_cast<uint8_t*>(self) + 0x6C, nullptr,
               22,
               static_cast<int>(queryResults[0]),
               static_cast<int>(queryResults[1]),
               static_cast<int>(queryResults[2]));

    return 1;
}

HOOK(0x004BFED0, S_004bc930_DispatchActionCode22, "auto");
