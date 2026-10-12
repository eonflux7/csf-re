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

CSF_FUNC(int, __fastcall, PrtStd_ApplyPositionDelta, 0x006F44C0, (void* self, void* edx, float* a2));

using PrtStdGetPositionFn = float* (__fastcall*)(void* self, void* edx);
using PrtStdSetPositionFn = void (__fastcall*)(void* self, void* edx, float* newPos, int extra);

// Sub-object at +0x20 of the particle atomic: updates its position and, when requested
// and the position actually changed, forwards the position delta to PrtStd_ApplyPositionDelta.
static void __fastcall PrtStdParticles_UpdateAtomicPosition(uint32_t* self, void* /*edx*/, float* newPos, int a3, char a4)
{
    void* atomic = reinterpret_cast<uint8_t*>(self) - 0x20;

    // Slot 0x48 / 4 = 18: read current position (pointer to float[3]).
    float* currentPos = reinterpret_cast<PrtStdGetPositionFn>(CSF_AT(void**, atomic, 0)[18])(atomic, nullptr);
    float oldX = currentPos[0];
    float oldY = currentPos[1];
    float oldZ = currentPos[2];

    // Slot 0x3C / 4 = 15: set the new position.
    reinterpret_cast<PrtStdSetPositionFn>(CSF_AT(void**, atomic, 0)[15])(atomic, nullptr, newPos, a3);

    if (a4 != 0 && (oldX != newPos[0] || oldY != newPos[1] || oldZ != newPos[2]))
    {
        alignas(8) uint8_t deltaBytes[0x18];
        float* delta = reinterpret_cast<float*>(deltaBytes);
        delta[0] = newPos[0] - oldX;
        delta[1] = newPos[1] - oldY;
        delta[2] = newPos[2] - oldZ;
        PrtStd_ApplyPositionDelta(atomic, nullptr, delta);
    }
}

HOOK(0x006F5150, PrtStdParticles_UpdateAtomicPosition, "auto");
