#include <cstdint>
#include "globals.h"
#include "hook.h"

// The original pushes three values per call: the entry index, then the two
// caller-supplied stack arguments (pushed as esi, then edi). The callee therefore
// takes (index, a2, a3). The earlier bind line had duplicate parameter names and
// two extra trailing arguments, which the original never pushes.
CSF_FUNC(void*, __cdecl, sub_42AD00, 0x0042AD00, (int index, int a2, int a3));

// Applies cheat entries 0, 1, 2, 3, 10, 11, then 12. Returns the result of the last call.
static void* __cdecl Cheats_ApplyCheatSet(int a2, int a3)
{
    sub_42AD00(0, a2, a3);
    sub_42AD00(1, a2, a3);
    sub_42AD00(2, a2, a3);
    sub_42AD00(3, a2, a3);
    sub_42AD00(10, a2, a3);
    sub_42AD00(11, a2, a3);
    return sub_42AD00(12, a2, a3);
}

HOOK(0x42AE00, Cheats_ApplyCheatSet, "auto");
