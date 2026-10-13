#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static uint32_t* __fastcall GfxBboxMath_BuildBoxCorners(
    void* self, void* /*edx*/, uint32_t* corners)
{
    corners[0]  = CSF_AT(uint32_t, self, 0x0C);
    corners[1]  = CSF_AT(uint32_t, self, 0x10);
    corners[2]  = CSF_AT(uint32_t, self, 0x14);

    corners[3]  = CSF_AT(uint32_t, self, 0x00);
    corners[4]  = CSF_AT(uint32_t, self, 0x10);
    corners[5]  = CSF_AT(uint32_t, self, 0x14);

    corners[6]  = CSF_AT(uint32_t, self, 0x00);
    corners[7]  = CSF_AT(uint32_t, self, 0x04);
    corners[8]  = CSF_AT(uint32_t, self, 0x14);

    corners[9]  = CSF_AT(uint32_t, self, 0x0C);
    corners[10] = CSF_AT(uint32_t, self, 0x04);
    corners[11] = CSF_AT(uint32_t, self, 0x14);

    corners[12] = CSF_AT(uint32_t, self, 0x0C);
    corners[13] = CSF_AT(uint32_t, self, 0x10);
    corners[14] = CSF_AT(uint32_t, self, 0x08);

    corners[15] = CSF_AT(uint32_t, self, 0x00);
    corners[16] = CSF_AT(uint32_t, self, 0x10);
    corners[17] = CSF_AT(uint32_t, self, 0x08);

    corners[18] = CSF_AT(uint32_t, self, 0x00);
    corners[19] = CSF_AT(uint32_t, self, 0x04);
    corners[20] = CSF_AT(uint32_t, self, 0x08);

    corners[21] = CSF_AT(uint32_t, self, 0x0C);
    corners[22] = CSF_AT(uint32_t, self, 0x04);
    corners[23] = CSF_AT(uint32_t, self, 0x08);

    return corners;
}

HOOK(0x006AB510, GfxBboxMath_BuildBoxCorners, "auto");
