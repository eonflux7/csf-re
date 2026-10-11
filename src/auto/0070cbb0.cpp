#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwV3dTransformPoints, 0x00623040, (int a1, int a2, int a3, int a4));
CSF_FUNC(int, __cdecl, RwV3dTransformVectors, 0x006230A0, (int a1, int a2, int a3, int a4));
CSF_FUNC(uint32_t*, __fastcall, S_00526d40_ConvertDirection, 0x006ABA10, (S_00526d40* self, void* edx, uint32_t* output));

static int __fastcall Water_TransformPlane(char* self, void* /*edx*/, int* arg0, int a3)
{
    alignas(8) uint8_t convertedBytes[0xC];
    alignas(8) uint8_t directionBytes[0xC];
    alignas(8) uint8_t pointBytes[0xC];
    alignas(8) uint8_t vectorBytes[0xC];

    uint32_t* vectorOut = reinterpret_cast<uint32_t*>(vectorBytes);
    uint32_t* directionIn = reinterpret_cast<uint32_t*>(directionBytes);
    uint32_t* pointOut = reinterpret_cast<uint32_t*>(pointBytes);
    uint32_t* converted = reinterpret_cast<uint32_t*>(convertedBytes);

    vectorOut[0] = 0;
    vectorOut[1] = 0;
    vectorOut[2] = 0;
    RwV3dTransformVectors((int)(uintptr_t)vectorOut, (int)(uintptr_t)(self + 12), 1, *arg0);

    directionIn[0] = vectorOut[0];
    directionIn[1] = vectorOut[1];
    directionIn[2] = vectorOut[2];

    pointOut[0] = 0;
    pointOut[1] = 0;
    pointOut[2] = 0;
    RwV3dTransformPoints((int)(uintptr_t)pointOut, (int)(uintptr_t)self, 1, *arg0);

    uint8_t* out = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(a3));
    CSF_AT(uint32_t, out, 0) = pointOut[0];
    CSF_AT(uint32_t, out, 4) = pointOut[1];
    CSF_AT(uint32_t, out, 8) = pointOut[2];

    uint32_t* dir = S_00526d40_ConvertDirection(reinterpret_cast<S_00526d40*>(directionBytes), nullptr, converted);
    CSF_AT(uint32_t, out, 12) = dir[0];
    CSF_AT(uint32_t, out, 16) = dir[1];
    uint32_t result = dir[2];
    CSF_AT(uint8_t, out, 40) = 1;
    CSF_AT(uint32_t, out, 20) = result;
    return (int)result;
}

HOOK(0x0070CBB0, Water_TransformPlane, "auto");
