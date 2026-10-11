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

CSF_FUNC(int*, __fastcall, sub_6C48C0, 0x006C48C0, (void* self, void* edx, int* a2, int a3));

// __thiscall: ecx = self, stack: a2, a3; ret 8.
// If self->count (+0x1C) is nonzero and a3 == 4, forwards the last 12-byte entry
// of the table at self->table (+0x14) to sub_6C48C0 with (a2, 4); otherwise zeroes *a2.
// Returns a2 in both paths.
static int* __fastcall GfxSkin_CopyLastLinkedEntryOrZero(void* self, void* /*edx*/, int* a2, int a3)
{
    if (CSF_AT(int, self, 0x1c) && a3 == 4)
    {
        C_0085e7b8* lastEntry = CSF_AT(C_0085e7b8*, CSF_AT(uint8_t*, self, 0x14),
                                       CSF_AT(int, self, 0x1c) * 12 - 12);
        sub_6C48C0(lastEntry, nullptr, a2, 4);
        return a2;
    }

    *a2 = 0;
    return a2;
}

HOOK(0x6C7BF0, GfxSkin_CopyLastLinkedEntryOrZero, "auto");
