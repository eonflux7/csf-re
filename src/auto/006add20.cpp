#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwMatrixUpdate, 0x00624620, (int a1));

// Copies three dwords from the source array into the matrix block at dst.
static void CopyThreeMatrixDwords(uint8_t* dst, const uint32_t* values)
{
    CSF_AT(uint32_t, dst, 0) = values[0];
    CSF_AT(uint32_t, dst, 4) = values[1];
    CSF_AT(uint32_t, dst, 8) = values[2];
}

// mode 1: write the triple at the matrix base, then refresh.
// mode 2: write the triple at base+16, then refresh and return the update result.
// mode 3: write the triple at base+32, then refresh.
// any other mode: return (mode - 3) without touching memory.
static int __fastcall S_006add20_SetMatrixSlot(S_006add20* self, void* /*edx*/, int mode, uint32_t* values)
{
    uint8_t* matrixBase = reinterpret_cast<uint8_t*>(self->field_0000);
    int offset = 0;

    if (mode == 1)
    {
        offset = 0;
    }
    else if (mode == 2)
    {
        CopyThreeMatrixDwords(matrixBase + 16, values);
        return RwMatrixUpdate(static_cast<int>(self->field_0000));
    }
    else
    {
        if (mode != 3)
            return mode - 3;
        offset = 32;
    }

    CopyThreeMatrixDwords(matrixBase + offset, values);
    return RwMatrixUpdate(static_cast<int>(self->field_0000));
}

HOOK(0x006ADD20, S_006add20_SetMatrixSlot, "auto");
