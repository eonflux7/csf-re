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

CSF_FUNC(int, __cdecl, Mem_AllocDebug, 0x00690DC0, (std::size_t size, int flags, int tag));

// Allocates a block from the game heap through the debug allocator, with the
// "Desconocido" (unknown owner) label and a zero tag.
static int __cdecl CoreMemory_AllocUntagged(std::size_t size)
{
    return Mem_AllocDebug(size,
                          static_cast<int>(reinterpret_cast<uintptr_t>(CSF_STR(0x0085E430, "Desconocido"))),
                          0);
}

HOOK(0x006910F0, CoreMemory_AllocUntagged, "auto");
