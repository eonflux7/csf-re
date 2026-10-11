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

// Constants referenced by the original (bound by address).
CSF_GLOBAL(float, g_GravityAccel, 0x0082E0EC);   // 980.0f multiplier
CSF_GLOBAL(float, g_WheelMinLimit, 0x0082E254);  // -98.0f lower bound

// Callees of the original.
CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_ConfigureFocusEntry, 0x0048E460, (void* self, void* edx, int* a2));

// Updates the wheel offset (field_0064) from the input record {scale, mode}
// and pushes the resulting focus position to the scene loader.
static int __fastcall C_0082e1e8_UpdateWheelOffset(C_0082e1e8* self, void* /*edx*/, int a2)
{
    const int mode = reinterpret_cast<const int*>(static_cast<std::uintptr_t>(a2))[1];

    if (mode >= 2) {
        self->field_0064 = 0.0f;
        return mode;
    }

    if (mode != 0) {
        // Clamp to -98 when the current offset is at or above the limit.
        if (self->field_0064 >= g_WheelMinLimit) {
            self->field_0064 = -98.0f;
        }
    } else {
        const float inputScale = *reinterpret_cast<const float*>(static_cast<std::uintptr_t>(a2));
        self->field_0064 = self->field_0064 - inputScale * g_GravityAccel;
    }

    const int positionVector = GetObjectPositionVector(
        reinterpret_cast<void*>(self->field_0004), nullptr);
    const int* pos = reinterpret_cast<const int*>(static_cast<std::uintptr_t>(positionVector));

    const float inputScale = *reinterpret_cast<const float*>(static_cast<std::uintptr_t>(a2));
    const float scaledOffset = self->field_0064 * inputScale;

    int focusEntry[3];
    focusEntry[0] = pos[0];
    const float adjustedY = scaledOffset + *reinterpret_cast<const float*>(&pos[1]);
    std::memcpy(&focusEntry[1], &adjustedY, sizeof(float));
    focusEntry[2] = pos[2];

    return SceneLoader_ConfigureFocusEntry(
        reinterpret_cast<void*>(self->field_0004), nullptr, focusEntry);
}

HOOK(0x4190C0, C_0082e1e8_UpdateWheelOffset, "auto");
