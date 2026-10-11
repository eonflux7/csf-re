#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_ProximityMidDistanceSquared, 0x8339A8);

// Casts a segment (origin + direction * scale) through the collision object stored at this+8
// (vtable slot 45) and, on a hit, writes position, normal and distance to the output array.
static int __fastcall Water_CastRay(void* self, void* /*edx*/, float* ray, float* out, int /*unused*/)
{
    alignas(8) uint8_t segBytes[0x24];
    alignas(8) uint8_t normalBytes[0xC];
    alignas(8) uint8_t hitPosBytes[0xC];

    float* origin = ray + 3;
    void* raySlot = ray; // original passes the address of its first stack argument

    float dx = ray[0] * g_ProximityMidDistanceSquared;
    float dy = ray[1] * g_ProximityMidDistanceSquared;
    float dz = ray[2] * g_ProximityMidDistanceSquared;

    void* collider = CSF_AT(void*, self, 8);

    CSF_AT(float, segBytes, 0x00) = origin[0];
    CSF_AT(float, segBytes, 0x04) = origin[1];
    CSF_AT(float, segBytes, 0x08) = origin[2];

    CSF_AT(float, hitPosBytes, 0x0) = 0.0f;
    CSF_AT(float, hitPosBytes, 0x4) = 0.0f;

    float endX = dx + origin[0];
    CSF_AT(float, segBytes, 0x18) = endX;
    CSF_AT(float, segBytes, 0x0C) = endX;

    CSF_AT(float, hitPosBytes, 0x8) = 0.0f;
    CSF_AT(int, normalBytes, 0x0) = 0;
    CSF_AT(int, normalBytes, 0x4) = 0;

    float endY = dy + origin[1];
    CSF_AT(float, segBytes, 0x1C) = endY;
    CSF_AT(float, segBytes, 0x10) = endY;

    CSF_AT(int, normalBytes, 0x8) = 0;

    float endZ = dz + origin[2];
    CSF_AT(float, segBytes, 0x20) = endZ;
    CSF_AT(float, segBytes, 0x14) = endZ;

    typedef int(__fastcall * CastFn)(void*, void*, void*, void*, void*, void*, int);
    CastFn cast = (CastFn)CSF_AT(void**, collider, 0)[0xB4 / 4];
    int result = cast(collider, nullptr, segBytes, hitPosBytes, normalBytes, &raySlot, 0);

    if ((uint8_t)result != 0) {
        float px = CSF_AT(float, hitPosBytes, 0x0);
        float py = CSF_AT(float, hitPosBytes, 0x4);
        float pz = CSF_AT(float, hitPosBytes, 0x8);
        out[0] = px;
        out[1] = py;
        out[2] = pz;
        CSF_AT(int, out, 0x0C) = CSF_AT(int, normalBytes, 0x0);
        CSF_AT(int, out, 0x10) = CSF_AT(int, normalBytes, 0x4);
        CSF_AT(int, out, 0x14) = CSF_AT(int, normalBytes, 0x8);

        float ex = px - origin[0];
        float ey = py - origin[1];
        float ez = pz - origin[2];
        out[6] = (float)sqrt((double)(ex * ex + ey * ey + ez * ez));
    }
    return result;
}

HOOK(0x70C7D0, Water_CastRay, "auto");
