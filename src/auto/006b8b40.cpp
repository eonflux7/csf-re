#include <cstdint>
#include <cstddef>
#include <cmath>
#include "globals.h"
#include "hook.h"

// Tolerance constant used by the original for near-zero comparisons.
CSF_GLOBAL(float, g_NearZeroEpsilon, 0x008603A0);

// Returns true when the sub-record stored at this+0x10 exists and either of
// its floats at +0x28 / +0x2C is at least the near-zero epsilon in magnitude.
static bool __fastcall GfxObjectProps_HasNonZeroOffsetXY(void* self, void* /*edx*/) {
    uint32_t subRecordAddr = CSF_AT(uint32_t, self, 0x10);
    if (subRecordAddr == 0) {
        return false;
    }

    void* subRecord = reinterpret_cast<void*>(subRecordAddr);
    float offsetX = CSF_AT(float, subRecord, 0x28);
    if (std::fabs(offsetX) >= g_NearZeroEpsilon) {
        return true;
    }

    float offsetY = CSF_AT(float, subRecord, 0x2C);
    return std::fabs(offsetY) >= g_NearZeroEpsilon;
}

HOOK(0x6B8B40, GfxObjectProps_HasNonZeroOffsetXY, "auto");
