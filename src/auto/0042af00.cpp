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

// Disassembly pushes exactly 3 stack args per call (id, a1, a2), so bind with 3.
CSF_FUNC(void*, __cdecl, sub_42AD00, 0x0042AD00, (int a3, int a4, int a5));

// Applies the cheat-entry dispatcher to the fixed list of cheat ids, in original order.
// Returns the result of the final dispatch call (id 31).
static void* __cdecl Cheats_DispatchCheatIds(int a1, int a2)
{
    sub_42AD00(14, a1, a2);
    sub_42AD00(15, a1, a2);
    sub_42AD00(20, a1, a2);
    sub_42AD00(21, a1, a2);
    sub_42AD00(22, a1, a2);
    sub_42AD00(23, a1, a2);
    sub_42AD00(26, a1, a2);
    sub_42AD00(8, a1, a2);
    sub_42AD00(28, a1, a2);
    sub_42AD00(29, a1, a2);
    sub_42AD00(30, a1, a2);
    return sub_42AD00(31, a1, a2);
}

HOOK(0x0042AF00, Cheats_DispatchCheatIds, "auto");
