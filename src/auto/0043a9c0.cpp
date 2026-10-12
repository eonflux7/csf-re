#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(float*, __fastcall, NormalizeVector2, 0x006AEE00, (void* self, void* edx));

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

// The original uses an inline fsqrt; the CRT sqrt symbol is not available to the
// hook DLL, so use a compiler builtin that always expands to the instruction.
static inline float Minimap_SquareRoot(float value)
{
#if defined(__has_builtin) && __has_builtin(__builtin_elementwise_sqrt)
    return __builtin_elementwise_sqrt(value);
#else
    typedef float Vec4f __attribute__((__vector_size__(16)));
    Vec4f packed = { value, 0.0f, 0.0f, 0.0f };
    return __builtin_ia32_sqrtss(packed)[0];
#endif
}

// Computes the 2D (x,z) direction from `from` to `to`, normalized and scaled to
// half the object's extent (field_002c - field_0024). When `absolute` is false the
// length is additionally proportional to distance / refLength.
static float* __fastcall Minimap_ComputeScaledDirection(C_00830e5c* self, void* /*edx*/,
                                                        float* out, float* to, float* from,
                                                        float refLength, uint32_t absolute)
{
    float vec[2];
    vec[0] = to[0] - from[0];
    vec[1] = to[2] - from[2];

    int extent = (int)(self->field_002c - self->field_0024);

    float scale;
    if ((uint8_t)absolute) {
        scale = (float)extent * g_HalfDurationScale;
    } else {
        float distance = Minimap_SquareRoot(vec[1] * vec[1] + vec[0] * vec[0]);
        scale = distance * ((float)extent * g_HalfDurationScale) / refLength;
    }

    if (g_fZero != Minimap_SquareRoot(vec[1] * vec[1] + vec[0] * vec[0])) {
        NormalizeVector2(vec, nullptr);
        vec[0] = vec[0] * scale;
        vec[1] = vec[1] * scale;
    }

    out[0] = vec[0];
    out[1] = vec[1];
    return out;
}

HOOK(0x0043A9C0, Minimap_ComputeScaledDirection, "auto");
