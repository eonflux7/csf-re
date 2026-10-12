#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, S_00526d40_ConvertDirection, 0x006ABA10, (S_00526d40* self, void* edx, uint32_t* output));

static int __fastcall Geometry_ComputeTriangleResult(float* self, void* /*edx*/, float* origin, float* pointA, float* pointB)
{
    float edgeB[3];  // pointB - origin (x kept as local, y/z in buffer)
    float normal[3]; // cross product result (buffer passed to callee)
    float edgeA[3];  // pointA - origin (buffer passed to callee)

    float bx = pointB[0] - origin[0];
    float by = pointB[1] - origin[1];
    float bz = pointB[2] - origin[2];
    edgeB[0] = bx;
    edgeB[1] = by;
    edgeB[2] = bz;

    edgeA[0] = pointA[0] - origin[0];
    edgeA[1] = pointA[1] - origin[1];
    edgeA[2] = pointA[2] - origin[2];

    float cx = bz * edgeA[1] - edgeA[2] * by;
    float cy = edgeA[2] * bx - bz * edgeA[0];
    float cz = edgeA[0] * by - edgeA[1] * bx;

    normal[0] = cx;
    normal[1] = cy;
    normal[2] = cz;

    S_00526d40_ConvertDirection(reinterpret_cast<S_00526d40*>(normal), nullptr, reinterpret_cast<uint32_t*>(edgeA));

    CSF_AT(uint32_t, self, 0x00) = CSF_AT(uint32_t, origin, 0x00);
    CSF_AT(uint32_t, self, 0x04) = CSF_AT(uint32_t, origin, 0x04);
    CSF_AT(uint32_t, self, 0x08) = CSF_AT(uint32_t, origin, 0x08);

    uint32_t* converted = S_00526d40_ConvertDirection(reinterpret_cast<S_00526d40*>(edgeA), nullptr, reinterpret_cast<uint32_t*>(normal));

    CSF_AT(uint32_t, self, 0x0C) = converted[0];
    CSF_AT(uint32_t, self, 0x10) = converted[1];
    int result = static_cast<int>(converted[2]);
    CSF_AT(uint8_t, self, 0x28) = 1;
    CSF_AT(int, self, 0x14) = result;
    return result;
}

HOOK(0x004020F0, Geometry_ComputeTriangleResult, "auto");
