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

CSF_FUNC(int, __fastcall, GetTableEntryDwordByIndex, 0x004084C0, (void* self, void* , int index));
CSF_FUNC(double, __cdecl, GfxMatrix_ComputeArcCosine, 0x0040C980, (float a1));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(float*, __fastcall, GfxMatrix_GetEulerAngles, 0x006AE3A0, (float** self, void* , float* angles));
CSF_FUNC(float*, __fastcall, NormalizeVector2, 0x006AEE00, (float* self, void*));

CSF_GLOBAL(float, g_RadToDeg, 0x0083C25C);
CSF_GLOBAL(float, g_RotationWrapStep, 0x0082E1E4);

static bool __fastcall C_0083dd58_IsTargetOutsideFacingCone(
    C_0083dd58* self, void* /*edx*/, C_008342e8* target)
{
    // matBytes[0..3] holds the matrix pointer (ebp-8), matBytes[4] the release flag (ebp-4).
    alignas(8) uint8_t matBytes[8];
    alignas(8) uint8_t anglesBytes[0x20];
    float dir[2];

    void* owner = reinterpret_cast<void*>(self->field_0014);
    void* skin = CSF_AT(void*, owner, 0x4c);
    void* skinTarget =
        ((void*(__fastcall*)(void*, void*))CSF_AT(void**, skin, 0)[0x3c / 4])(
            skin, nullptr);

    int entry = GetTableEntryDwordByIndex(skinTarget, nullptr, 25);
    void* entryObject = reinterpret_cast<void*>(entry);
    ((void(__fastcall*)(void*, void*, void*))CSF_AT(void**, entryObject, 0)[0x3c / 4])(
        entryObject, nullptr, matBytes);

    void* matrix = reinterpret_cast<void*>(CSF_AT(std::uint32_t, matBytes, 0));
    float ownerX = CSF_AT(float, matrix, 0x30);
    float ownerZ = CSF_AT(float, matrix, 0x38);

    float* position = GetObjectPositionVector(target, nullptr);
    float targetX = position[0];
    float targetZ = position[2];

    float* angles = GfxMatrix_GetEulerAngles(
        reinterpret_cast<float**>(matBytes), nullptr,
        reinterpret_cast<float*>(anglesBytes));
    float yaw = angles[1];
    float sinYaw = (float)std::sin((double)yaw);
    float cosYaw = (float)std::cos((double)yaw);

    dir[0] = targetX - ownerX;
    dir[1] = targetZ - ownerZ;
    float* normalized = NormalizeVector2(dir, nullptr);
    float directionX = normalized[0];
    float directionZ = normalized[1];

    float dot = directionZ * cosYaw + sinYaw * directionX;
    double angleDegrees = GfxMatrix_ComputeArcCosine(dot) * (double)g_RadToDeg;
    bool result = angleDegrees > (double)g_RotationWrapStep;

    if (CSF_AT(uint8_t, matBytes, 4) != 0) {
        RwMatrixDestroy(
            reinterpret_cast<std::uint32_t*>(CSF_AT(std::uint32_t, matBytes, 0)));
    }
    return result;
}

HOOK(0x004D5630, C_0083dd58_IsTargetOutsideFacingCone, "auto");
