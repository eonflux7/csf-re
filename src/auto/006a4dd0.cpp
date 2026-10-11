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

// _free from the MSVC CRT, bound at the address the original calls.
CSF_FUNC(void, __cdecl, _free, 0x007B17CC, (void* Block));

// Releases a block previously obtained from the zlib/log allocator.
// The first argument (a1) is not used by the original and is ignored.
static void __cdecl ZlibLog_ReleaseBlock(int a1, void* Block)
{
    (void)a1;
    _free(Block);
}

HOOK(0x6A4DD0, ZlibLog_ReleaseBlock, "auto");
