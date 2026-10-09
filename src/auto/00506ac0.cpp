#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// 0x00506AC0 (size 1 byte) - empty stub / nullsub; the original body is a bare `ret`.
// No stack arguments are popped (ret 0), consistent with __cdecl.
static void __cdecl SceneLoader_NoOp()
{
}

// LTCG callers (0x740AA0, 0x740AF0, 0x740B40) keep a value in ecx across the call: the original never writes ecx.
HOOK_KEEP_ECX(0x506AC0, SceneLoader_NoOp, 0, "auto");
