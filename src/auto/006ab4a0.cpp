#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Copies the six floats of a bounding box (min x/y/z at +0/+4/+8, max x/y/z
// at +0xC/+0x10/+0x14) out to six caller-supplied pointers. Returns the last
// value written (the min z component), which the original leaves in eax.
static int __fastcall BBoxGetExtents(void* self, void* /*edx*/,
                                     uint32_t* outMinX, uint32_t* outMaxX,
                                     uint32_t* outMinY, uint32_t* outMaxY,
                                     uint32_t* outMinZ, uint32_t* outMaxZ)
{
    *outMaxX = CSF_AT(uint32_t, self, 0x0C);
    *outMaxY = CSF_AT(uint32_t, self, 0x10);
    *outMaxZ = CSF_AT(uint32_t, self, 0x14);
    *outMinX = CSF_AT(uint32_t, self, 0x00);
    *outMinY = CSF_AT(uint32_t, self, 0x04);
    int result = static_cast<int>(CSF_AT(uint32_t, self, 0x08));
    *outMinZ = static_cast<uint32_t>(result);
    return result;
}

HOOK(0x6AB4A0, BBoxGetExtents, "auto");
