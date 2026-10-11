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

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);

// Returns (fmt[2] * 8 / (fmt[3] & 0xFF)) / fmt[0], where fmt is the 7-dword block
// at +0x2C of the block hanging off this->field_0004 at +0x50.
// Returns 0.0 when that block pointer is null.
// minsize: the original copies the 7-dword block as a single block move (rep movsd).
__attribute__((minsize))
static double __fastcall C_00863dd0_GetBitrateToSampleRatio(C_00863dd0* self, void* /*edx*/)
{
    uint32_t formatBlockAddr = CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0004), 0x50);
    if (formatBlockAddr == 0)
        return g_fZero;

    const uint32_t* formatBlock =
        reinterpret_cast<const uint32_t*>(reinterpret_cast<const uint8_t*>(static_cast<uintptr_t>(formatBlockAddr)) + 0x2C);

    // Snapshot the whole 7-dword block, then work on the copy.
    uint32_t fmt[7];
    std::memcpy(fmt, formatBlock, sizeof(fmt));

    // Unsigned integer math: (fmt[2] << 3) / low byte of fmt[3].
    uint32_t bitsTimes8 = fmt[2] << 3;
    uint32_t divisor = fmt[3] & 0xFFu;
    uint32_t quotient = bitsTimes8 / divisor;

    double numerator = static_cast<double>(quotient);
    double denominator = static_cast<double>(fmt[0]);
    return numerator / denominator;
}

HOOK(0x007125D0, C_00863dd0_GetBitrateToSampleRatio, "auto");
