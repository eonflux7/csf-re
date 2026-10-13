#include <cstdint>
#include "hook.h"

// Bounding-box overlap test. self = box A (6 floats: min xyz, max xyz laid out
// as [0..2], [3..5]); other = box B. Each axis test is written as the negation of
// the strict ordered comparison that the x87 fcomp/test sequence rejects on, so a
// NaN operand passes that axis, exactly as the original's C0/C2 flag tests do.
static bool __fastcall GfxBbox_TestBoundsOverlap(float* self, void* /*edx*/, float* other)
{
    if (other[3] > self[0])   return false;   // other.max.x vs self.min.x
    if (other[0] < self[3])   return false;   // other.min.x vs self.max.x
    if (other[4] > self[1])   return false;   // other.max.y vs self.min.y
    if (other[1] < self[4])   return false;   // other.min.y vs self.max.y
    if (other[5] > self[2])   return false;   // other.max.z vs self.min.z
    if (other[2] < self[5])   return false;   // other.min.z vs self.max.z
    return true;
}

HOOK(0x006AB740, GfxBbox_TestBoundsOverlap, "auto");
