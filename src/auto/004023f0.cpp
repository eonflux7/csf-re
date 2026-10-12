#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, S_00526d40_ConvertDirection, 0x006ABA10, (S_00526d40* self, void* edx, uint32_t* output));

static uint32_t* __fastcall Geometry_BuildPlaneFromTriangle(S_00526d40* self, void* /*edx*/, float* origin, float* pointB, float* pointC)
{
    // Stack objects of the original frame (12 bytes each)
    alignas(8) uint8_t crossBytes[12];
    alignas(8) uint8_t edgeBytes[12];
    S_00526d40* cross = reinterpret_cast<S_00526d40*>(crossBytes);
    S_00526d40* edge = reinterpret_cast<S_00526d40*>(edgeBytes);

    self->field_0000 = 0;
    CSF_AT(uint32_t, self, 4) = 0;
    CSF_AT(uint32_t, self, 8) = 0;
    CSF_AT(uint32_t, self, 0xC) = 0;
    CSF_AT(uint32_t, self, 0x10) = 0;
    CSF_AT(uint32_t, self, 0x14) = 0;

    float cx = pointC[0] - origin[0];
    float cy = pointC[1] - origin[1];
    float cz = pointC[2] - origin[2];

    float bx = pointB[0] - origin[0];
    float by = pointB[1] - origin[1];
    float bz = pointB[2] - origin[2];

    float nx = cz * by - bz * cy;
    float ny = bz * cx - cz * bx;
    float nz = bx * cy - by * cx;

    CSF_AT(float, crossBytes, 0) = nx;
    CSF_AT(float, crossBytes, 4) = ny;
    CSF_AT(float, crossBytes, 8) = nz;

    // edge area is reused as output of the first conversion
    CSF_AT(float, edgeBytes, 0) = bx;
    CSF_AT(float, edgeBytes, 4) = by;
    CSF_AT(float, edgeBytes, 8) = nz;

    S_00526d40_ConvertDirection(cross, nullptr, reinterpret_cast<uint32_t*>(edge));

    self->field_0000 = CSF_AT(uint32_t, origin, 0);
    CSF_AT(uint32_t, self, 4) = CSF_AT(uint32_t, origin, 4);
    CSF_AT(uint32_t, self, 8) = CSF_AT(uint32_t, origin, 8);

    uint32_t* result = S_00526d40_ConvertDirection(edge, nullptr, reinterpret_cast<uint32_t*>(cross));

    CSF_AT(uint32_t, self, 0xC) = result[0];
    CSF_AT(uint32_t, self, 0x10) = result[1];
    CSF_AT(uint32_t, self, 0x14) = result[2];
    CSF_AT(uint8_t, self, 0x28) = 1;

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x004023F0, Geometry_BuildPlaneFromTriangle, "auto");
