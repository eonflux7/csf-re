#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

// Scales the first and third components of the input vector by 1/field_007c
// (when nonzero) and the second and fourth by 1/field_0080 (when nonzero),
// then dispatches through vtable slot 0x4C with the scaled vector.
static int __fastcall C_00867260_ApplyInverseScaleAndDispatch(C_00867260* self, void* /*edx*/, int* a2)
{
    float v6 = static_cast<float>(a2[0]);
    float v7 = static_cast<float>(a2[1]);
    float v8 = static_cast<float>(a2[2]);
    uint32_t scaleX = CSF_AT(uint32_t, self, 0x7c);
    float v9 = static_cast<float>(a2[3]);

    if (scaleX != 0) {
        float inverseX = g_TransitionOne / static_cast<float>(static_cast<int32_t>(CSF_AT(uint32_t, self, 0x7c)));
        v6 = v6 * inverseX;
        v8 = inverseX * v8;
    }
    if (CSF_AT(uint32_t, self, 0x80) != 0) {
        float inverseY = g_TransitionOne / static_cast<float>(static_cast<int32_t>(CSF_AT(uint32_t, self, 0x80)));
        v7 = v7 * inverseY;
        v9 = inverseY * v9;
    }

    float scaledVector[4] = { v6, v7, v8, v9 };
    return ((int(__fastcall*)(void*, void*, float*))CSF_AT(void**, self, 0)[0x4C / 4])(self, nullptr, scaledVector);
}

HOOK(0x7376A0, C_00867260_ApplyInverseScaleAndDispatch, "auto");
