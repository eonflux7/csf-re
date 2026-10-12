#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

// Scales the first and third components of the input vector by 1/field_002c
// (when nonzero) and the second and fourth by 1/field_0030 (when nonzero),
// then dispatches through vtable slot 0x38 with the scaled vector.
static int __fastcall C_008673b0_ApplyInverseScaleAndDispatch(C_008673b0* self, void* /*edx*/, int* a2)
{
    float v6 = static_cast<float>(a2[0]);
    float v7 = static_cast<float>(a2[1]);
    float v8 = static_cast<float>(a2[2]);
    uint32_t scaleX = self->field_002c;
    float v9 = static_cast<float>(a2[3]);

    if (scaleX != 0) {
        float inverseX = g_TransitionOne / static_cast<float>(static_cast<int32_t>(self->field_002c));
        v6 = v6 * inverseX;
        v8 = inverseX * v8;
    }
    if (self->field_0030 != 0) {
        float inverseY = g_TransitionOne / static_cast<float>(static_cast<int32_t>(self->field_0030));
        v7 = v7 * inverseY;
        v9 = inverseY * v9;
    }

    float scaledVector[4] = { v6, v7, v8, v9 };
    return ((int(__fastcall*)(void*, void*, float*))CSF_AT(void**, self, 0)[0x38 / 4])(self, nullptr, scaledVector);
}

HOOK(0x738DD0, C_008673b0_ApplyInverseScaleAndDispatch, "auto");
