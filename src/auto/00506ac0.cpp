#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// 0x00506AC0 (size 1 byte) - empty stub / nullsub; the original body is a bare `ret`.
// No stack arguments are popped (ret 0), consistent with __cdecl.
static void __cdecl Nullsub_506AC0()
{
}

HOOK(0x506AC0, Nullsub_506AC0, "auto");
