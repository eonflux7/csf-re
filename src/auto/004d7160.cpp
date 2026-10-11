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

// Callee: CharTask_LookupByOffset (0x004DB440), __fastcall, ret 4.
CSF_FUNC(int, __fastcall, CharTask_LookupByOffset, 0x004DB440, (void* self, void* edx, int a2));

// Returns the name lookup result for this object: looks up the CharTask entry
// at (stored field at +0x0C) + 0x54 through CharTask_LookupByOffset.
// Original is __thiscall with two stack args (a2, a3) popped by the callee (ret 8).
static int __fastcall TDarObjeto_GetName(TDarObjeto* self, void* /*edx*/, int a2, int a3)
{
    (void)a2;
    (void)a3;
    int offset = CSF_AT(int, self, 0x0C) + 0x54;
    return CharTask_LookupByOffset(self, nullptr, offset);
}

HOOK(0x004D7160, TDarObjeto_GetName, "auto");
