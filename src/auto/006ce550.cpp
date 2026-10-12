#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"

// Bound original: sub_6CE3A0 (rets=4, __fastcall with one stack arg).
CSF_FUNC(char*, __fastcall, GfxCollisionShapes_CopyElement, 0x006CE3A0, (void* self, void* edx, int a2));

// Copies elements from [a1, a2) backwards into the block ending at a3.
// Each element is 100 bytes (25 dwords). The first 0x64 bytes are handled by
// GfxCollisionShapes_CopyElement; the dword at +0x60 is copied afterwards.
// Returns the destination of the last element written, or a3 if the range is empty.
static uint32_t* __cdecl GfxCollisionShapes_CopyRangeBackward(uint32_t* rangeBegin, uint32_t* rangeEnd, uint32_t* destEnd, int /*unused*/)
{
    if (rangeBegin == rangeEnd)
        return destEnd;

    uint8_t* srcCursor = reinterpret_cast<uint8_t*>(rangeEnd);
    uint8_t* dstCursor = reinterpret_cast<uint8_t*>(destEnd);

    do
    {
        srcCursor -= 100;
        dstCursor -= 100;
        GfxCollisionShapes_CopyElement(dstCursor, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(srcCursor)));
        CSF_AT(uint32_t, dstCursor, 0x60) = CSF_AT(uint32_t, srcCursor, 0x60);
    } while (srcCursor != reinterpret_cast<uint8_t*>(rangeBegin));

    return reinterpret_cast<uint32_t*>(dstCursor);
}

HOOK(0x006CE550, GfxCollisionShapes_CopyRangeBackward, "auto");
