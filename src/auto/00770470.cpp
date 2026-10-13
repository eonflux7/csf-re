#include "hook.h"
#include "globals.h"
#include "core/memory.h"

// Returns non-zero when a2 lies in the half-open interval [min, min + extent),
// where min = this[0x50] and extent = this[0x54] (floats in the sound-area record).
// Both tests are written the way the original branches: an unordered compare (NaN height) returns 0.
static int __fastcall SoundAreas_IsHeightInRange(float* self, void* /*edx*/, float height) {
    const float rangeMin = CSF_AT(float, self, 0x50);
    const float rangeExtent = CSF_AT(float, self, 0x54);

    if (!(height >= rangeMin)) {
        return 0;
    }
    // the sum stays on the x87 stack in the original: not rounded to float before the compare
    if (!(static_cast<double>(rangeExtent) + static_cast<double>(rangeMin) > height)) {
        return 0;
    }
    return 1;
}

HOOK_KEEP_ECX(0x770470, SoundAreas_IsHeightInRange, 1, "bdd_sound_areas");
