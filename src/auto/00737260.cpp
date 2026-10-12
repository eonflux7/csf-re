#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

// Recomputes the two interpolation bounds (0x34 / 0x38) from the stage in 0x54.
// Returns the value the original leaves in eax: the raw bits of 0x30 for stage 1,
// stage-1 for stages 2-4, and the raw bits of 0x2C for the default case.
static int __fastcall C_00867260_ComputeTransitionBounds(C_00867260* self, void* /*edx*/) {
    int result = CSF_AT(int, self, 0x54) - 1;

    switch (CSF_AT(uint32_t, self, 0x54)) {
    case 1u:
        result = static_cast<int>(CSF_AT(uint32_t, self, 0x30));
        CSF_AT(float, self, 0x34) = CSF_AT(float, self, 0x2C) - CSF_AT(float, self, 0x44) - g_TransitionOne;
        CSF_AT(uint32_t, self, 0x38) = CSF_AT(uint32_t, self, 0x30);
        break;

    case 2u:
        CSF_AT(uint32_t, self, 0x34) = CSF_AT(uint32_t, self, 0x2C);
        CSF_AT(float, self, 0x38) = CSF_AT(float, self, 0x30) - CSF_AT(float, self, 0x48) - g_TransitionOne;
        break;

    case 3u:
        CSF_AT(float, self, 0x34) = CSF_AT(float, self, 0x2C) - CSF_AT(float, self, 0x44) - 1.0f;
        CSF_AT(float, self, 0x38) = CSF_AT(float, self, 0x30) - CSF_AT(float, self, 0x48) - 1.0f;
        break;

    case 4u:
        CSF_AT(float, self, 0x34) = CSF_AT(float, self, 0x2C) - CSF_AT(float, self, 0x44) * g_HalfDurationScale;
        CSF_AT(float, self, 0x38) = CSF_AT(float, self, 0x30) - CSF_AT(float, self, 0x48) * g_HalfDurationScale;
        break;

    default:
        result = static_cast<int>(CSF_AT(uint32_t, self, 0x2C));
        CSF_AT(uint32_t, self, 0x34) = CSF_AT(uint32_t, self, 0x2C);
        CSF_AT(uint32_t, self, 0x38) = CSF_AT(uint32_t, self, 0x30);
        break;
    }

    return result;
}

HOOK(0x00737260, C_00867260_ComputeTransitionBounds, "auto");
