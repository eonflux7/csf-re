#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_ProximityMidDistanceSquared, 0x8339A8);

// Builds a segment (origin, origin + direction * scale) from a 6-float input and
// forwards it to virtual slot 45 of the object stored at this+8.
static int __fastcall Water_QueryScaledSegment(void* self, void* /*edx*/, int a2, int a3, int a4)
{
    const float* in = reinterpret_cast<const float*>(a2);
    void* target = CSF_AT(void*, self, 8);

    alignas(8) uint8_t segBytes[0x18];
    alignas(8) uint8_t zeroA[0xC];
    alignas(8) uint8_t zeroB[0x18];

    float* seg = reinterpret_cast<float*>(segBytes);
    float* tail = reinterpret_cast<float*>(zeroB);

    float sx = in[0] * g_ProximityMidDistanceSquared;
    float sy = in[1] * g_ProximityMidDistanceSquared;
    float sz = in[2] * g_ProximityMidDistanceSquared;

    seg[0] = in[3];
    seg[1] = in[4];
    std::memset(zeroA, 0, 0xC);
    seg[2] = in[5];
    std::memset(zeroB, 0, 0xC);

    float ex = sx + in[3];
    tail[3] = ex;
    seg[3] = ex;
    float ey = sy + in[4];
    tail[4] = ey;
    float ez = sz + in[5];
    tail[5] = ez;
    seg[4] = ey;
    seg[5] = ez;

    typedef int(__fastcall * Fn)(void*, void*, void*, void*, void*, int*, int);
    Fn fn = reinterpret_cast<Fn>(CSF_AT(void**, target, 0)[45]);
    return fn(target, nullptr, segBytes, zeroA, zeroB, &a2, 0);
}

HOOK(0x70C720, Water_QueryScaledSegment, "auto");
