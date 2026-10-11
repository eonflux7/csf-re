#include <cstdint>
#include <cstddef>
#include "hook.h"

// Copies two 3-dword records into a 6-dword destination:
// dst[0..2] <- first, dst[3..5] <- second. Returns dst.
// Original: __thiscall, ret 8 (two stack args).
static uint32_t* __fastcall BuildQuerySegment(uint32_t* self, void* /*edx*/, const uint32_t* first, const uint32_t* second)
{
    self[0] = first[0];
    self[1] = first[1];
    self[2] = first[2];
    self[3] = second[0];
    self[4] = second[1];
    self[5] = second[2];
    return self;
}

HOOK(0x0040FB60, BuildQuerySegment, "auto");
