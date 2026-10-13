#include <cstddef>
#include "globals.h"
#include <cstdint>
#include "hook.h"

// Original callees, bound by address.
CSF_FUNC(int, __cdecl, Mem_AllocDebug, 0x00690DC0, (std::size_t size, int flags, int tag));
CSF_FUNC(int, __cdecl, AllocateAlignedBuffer, 0x00690EB0, (std::size_t size, int alignment, int option1, int option2));

// Allocation requests with a small size, or with one of the listed type/tag values,
// go to the debug allocator. Everything else gets a 64-byte aligned buffer.
static int __cdecl CoreMemory_AllocTagged(std::size_t size, std::uint16_t tag)
{
    if (size <= 0x20)
        return Mem_AllocDebug(size, 0, 0);

    bool isSpecialTag;
    if (tag > 0x1B) {
        isSpecialTag = (tag == 0x506);
    } else {
        if (tag == 0x1B || tag == 0x16)
            return Mem_AllocDebug(size, 0, 0);
        isSpecialTag = (tag == 0x18);
    }

    if (!isSpecialTag && tag != 0x11F)
        return AllocateAlignedBuffer(size, 64, 0, 0);

    return Mem_AllocDebug(size, 0, 0);
}

HOOK(0x00691120, CoreMemory_AllocTagged, "auto");
