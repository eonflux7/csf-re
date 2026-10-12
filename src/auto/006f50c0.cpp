#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Applies a position delta to the atomic's particle state (original 0x6F44C0, __thiscall, ret 4).
CSF_FUNC(int, __fastcall, RpPrtStd_ApplyPositionDelta, 0x006F44C0, (void* self, void* edx, float* a2));

// Updates the atomic's stored position when the new position differs from the current one.
// Calls the position setter (vtable slot 14); if updateDelta is set, also applies the
// difference (new - old) via RpPrtStd_ApplyPositionDelta.
static void __fastcall RpPrtStd_UpdateAtomicPosition(void* self, void* /*edx*/, float* newPosition, char updateDelta)
{
    // The atomic's base object sits 0x20 bytes before the sub-object passed as `this`.
    uint8_t* base = reinterpret_cast<uint8_t*>(self) - 0x20;

    // Slot 18 (offset 0x48) returns a pointer to the current position vector.
    float* currentPosition =
        ((float* (__fastcall*)(void*, void*))CSF_AT(void**, base, 0)[18])(base, nullptr);
    float currentX = currentPosition[0];
    float currentY = currentPosition[1];
    float currentZ = currentPosition[2];

    // NaN compares as "changed", matching the original's unordered-jump path.
    if (currentX != newPosition[0] || currentY != newPosition[1] || currentZ != newPosition[2])
    {
        // Slot 14 (offset 0x38): position setter taking the new position.
        ((void (__fastcall*)(void*, void*, float*))CSF_AT(void**, base, 0)[14])(base, nullptr, newPosition);

        if (updateDelta)
        {
            // Stack object the original gives the callee: 0x18 bytes.
            alignas(8) uint8_t deltaBytes[0x18];
            float* delta = reinterpret_cast<float*>(deltaBytes);
            delta[0] = newPosition[0] - currentX;
            delta[1] = newPosition[1] - currentY;
            delta[2] = newPosition[2] - currentZ;
            RpPrtStd_ApplyPositionDelta(base, nullptr, delta);
        }
    }
}

HOOK(0x006F50C0, RpPrtStd_UpdateAtomicPosition, "auto");
