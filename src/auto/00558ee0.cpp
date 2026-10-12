#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Per-entry copy helper at 0x558BA0. Called as __fastcall with self = destination
// sub-object (dst + 1) and a5 = source sub-object (src + 1); callee pops 4 bytes.
CSF_FUNC(int, __fastcall, SabotageEntry_CopyInner, 0x00558BA0, (void* self, void* edx, int a5));

// Copies a range of 0x14-byte (5-dword) entries from [srcBegin, srcEnd) to dstBegin.
// Each entry's first dword is copied directly, then the trailing 4-dword sub-object is
// copied through SabotageEntry_CopyInner. Returns the one-past-last destination entry,
// or dstBegin when the source range is empty.
static uint32_t* __cdecl CopySabotageEntryRange(uint32_t* srcBegin, uint32_t* srcEnd,
                                                uint32_t* dstBegin, int /*unused*/)
{
    uint32_t* src = srcBegin;
    if (src == srcEnd)
        return dstBegin;

    uint32_t* dst = dstBegin;
    do
    {
        *dst = *src;
        SabotageEntry_CopyInner(dst + 1, nullptr, reinterpret_cast<int>(src + 1));
        src += 5;
        dst += 5;
    } while (src != srcEnd);

    return dst;
}

HOOK(0x558EE0, CopySabotageEntryRange, "auto");
