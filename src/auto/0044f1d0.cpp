#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Returns 1 when the squared magnitude a2*a2 is at least the stored threshold
// at offset +4 of the object; 0 otherwise (including NaN, which the x87 flags
// report as "less than").
static int __fastcall SceneQueries_CheckSelectionLimit(float* self, void* /*edx*/, float a2) {
    float squared = a2 * a2;
    return squared >= CSF_AT(float, self, 0x04) ? 1 : 0;
}

HOOK(0x44F1D0, SceneQueries_CheckSelectionLimit, "auto");
