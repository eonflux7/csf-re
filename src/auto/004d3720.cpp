#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GfxTransform_GetRotatedPosition, 0x00409820, (void* self, void*));
CSF_FUNC(char*, __fastcall, GfxTransform_GetOtherPosition, 0x00655FF0, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_PositionDistanceSquaredLimit, 0x0083BCEC);

static int __fastcall C_0083bc28_IsObjectNearSkinMidpoint(
    C_0083bc28* self, void* /*edx*/, C_008342e8* object)
{
    void* owner = reinterpret_cast<void*>(self->field_0014);
    void* skin = reinterpret_cast<void*>(CSF_AT(uint32_t, owner, 0x4C));
    void* transform =
        reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, skin, 0)[0x3C / 4])(skin, nullptr);

    void* rotatedPosition = reinterpret_cast<void*>(
        GfxTransform_GetRotatedPosition(transform, nullptr));
    char* otherPosition = GfxTransform_GetOtherPosition(transform, nullptr);

    const float sumX = CSF_AT(float, rotatedPosition, 0x0)
                     + CSF_AT(float, otherPosition, 0x0);
    const float sumY = CSF_AT(float, rotatedPosition, 0x4)
                     + CSF_AT(float, otherPosition, 0x4);
    const float sumZ = CSF_AT(float, rotatedPosition, 0x8)
                     + CSF_AT(float, otherPosition, 0x8);

    const float midpointX = sumX * g_HalfDurationScale;
    const float midpointY = sumY * g_HalfDurationScale;
    const float midpointZ = sumZ * g_HalfDurationScale;

    float* position = GetObjectPositionVector(object, nullptr);
    const float deltaX = midpointX - position[0];
    const float deltaY = midpointY - position[1];
    const float deltaZ = midpointZ - position[2];

    return deltaZ * deltaZ + deltaY * deltaY + deltaX * deltaX
         < g_PositionDistanceSquaredLimit;
}

HOOK(0x004D3720, C_0083bc28_IsObjectNearSkinMidpoint, "auto");
