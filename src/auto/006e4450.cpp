#include <cstdint>
#include "globals.h"
#include "hook.h"

// Stores the record (base offset, scale) into a5 only when depth a4 is strictly
// smaller than the record's current stored depth at a5+4.
static int __cdecl GfxIm3d_StoreSmallerDepthRecord(uint32_t a1, uint32_t a2, uint32_t a3, float a4, uint32_t a5)
{
    (void)a1;
    int result = static_cast<int>(a3);

    if (a4 < CSF_AT(float, a5, 4))
    {
        uint32_t baseAddress = CSF_AT(uint32_t, a2, 4);
        int32_t index = CSF_AT(int32_t, a3, 24);
        CSF_AT(uint32_t, a5, 0) = baseAddress + static_cast<uint32_t>(index) * 8u;
        CSF_AT(float, a5, 4) = a4;
    }

    return result;
}

HOOK(0x6E4450, GfxIm3d_StoreSmallerDepthRecord, "auto");
