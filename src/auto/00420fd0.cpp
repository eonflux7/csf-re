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
#include "legacy/string.h"

CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GuiControl_GetPathPointRecord, 0x004A0F30, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, S_00526d40_ConvertDirection, 0x006ABA10, (S_00526d40* self, void* , uint32_t* output));
CSF_FUNC(double, __fastcall, S_00526d40_GetVectorAngleRadians, 0x006AB930, (S_00526d40* self, void*));

// Returns the heading angle (radians) from a reference point (a2) toward path point
// number a3 of the path stored in the provider at this->field_0008 + 0x25C.
// If a3 is past the end of the path, returns the cached value at this->field_0008 + 0x208.
static double __fastcall C_0082f9f8_GetDirectionAngleToPathPoint(C_0082f9f8* self, void* /*edx*/, float* referencePoint, uint32_t pathPointIndex)
{
    uint8_t* owner = CSF_AT(uint8_t*, self, 0x08);
    void* provider = owner + 0x25C;

    uint32_t pathPointCount = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(GetCachedObjectProvider(provider, nullptr)));
    if (pathPointIndex >= pathPointCount)
    {
        return static_cast<double>(CSF_AT(float, owner, 0x208));
    }

    float* pathPoint = reinterpret_cast<float*>(GuiControl_GetPathPointRecord(owner + 0x25C, nullptr, static_cast<int>(pathPointIndex)));

    // Direction vector from the reference point to the path point, stored as three floats.
    float deltaX = pathPoint[0] - referencePoint[0];
    float deltaY = pathPoint[1] - referencePoint[1];
    float deltaZ = pathPoint[2] - referencePoint[2];

    uint8_t directionBuffer[12];
    CSF_AT(float, directionBuffer, 0) = deltaX;
    CSF_AT(float, directionBuffer, 4) = deltaY;
    CSF_AT(float, directionBuffer, 8) = deltaZ;

    uint8_t normalizedBuffer[12];
    S_00526d40_ConvertDirection(reinterpret_cast<S_00526d40*>(directionBuffer), nullptr,
                                reinterpret_cast<uint32_t*>(normalizedBuffer));

    return S_00526d40_GetVectorAngleRadians(reinterpret_cast<S_00526d40*>(normalizedBuffer), nullptr);
}

HOOK(0x420FD0, C_0082f9f8_GetDirectionAngleToPathPoint, "auto");
