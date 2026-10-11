#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee: sub_6ADA10, __thiscall with 4 stack args (ret 16). Bound as __fastcall per packet.
CSF_FUNC(int, __fastcall, sub_6ADA10, 0x006ADA10, (void* self, void* edx, uint32_t* a2, uint32_t* a3, uint32_t* a4, uint32_t* a5));

// Splits the 15-dword source block passed in a2 into four local 3-dword vectors
// (elements 0-2, 4-6, 8-10, 12-14) and forwards them to sub_6ADA10 with the same `this`.
static int __fastcall GfxMatrix_ForwardSplitBlocks(S_006ada10* self, void* /*edx*/, uint32_t* a2)
{
    uint32_t v7[3];
    uint32_t a3[3];
    uint32_t a4[3];
    uint32_t a5[3];

    v7[0] = a2[0];
    v7[1] = a2[1];
    v7[2] = a2[2];

    a3[0] = a2[4];
    a3[1] = a2[5];
    a3[2] = a2[6];

    a4[0] = a2[8];
    a4[1] = a2[9];
    a4[2] = a2[10];

    a5[0] = a2[12];
    a5[1] = a2[13];
    a5[2] = a2[14];

    return sub_6ADA10(self, nullptr, v7, a3, a4, a5);
}

HOOK(0x006ADA90, GfxMatrix_ForwardSplitBlocks, "auto");
