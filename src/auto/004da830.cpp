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

CSF_FUNC(int, __fastcall, GetTableEntryDwordByIndex, 0x004084C0, (void* self, void* /*edx*/, int index));
CSF_FUNC(double, __cdecl, CalculateArcCosine, 0x0040C980, (float a1));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(float*, __fastcall, GfxMatrix_GetEulerAngles, 0x006AE3A0, (float** self, void* /*edx*/, float* angles));
CSF_FUNC(float*, __fastcall, NormalizeVector2, 0x006AEE00, (void* self, void* edx));

CSF_GLOBAL(float, g_RadToDeg, 0x0083DD38);
CSF_GLOBAL(float, g_RotationWrapStep, 0x0082E1E4);

// Returns true when the angle between the owner's facing and the direction to a2 exceeds the threshold.
static bool __fastcall C_0083dd58_IsTargetOutsideFacingCone(C_0083dd58* self, void* /*edx*/, C_008342e8* a2)
{
    alignas(8) uint8_t matBytes[8];
    alignas(8) uint8_t anglesBytes[0x20];
    float dir[2];

    void* obj14 = reinterpret_cast<void*>(self->field_0014);
    void* obj4c = CSF_AT(void*, obj14, 0x4c);
    void* skinTarget = ((void*(__fastcall*)(void*, void*))CSF_AT(void**, obj4c, 0)[0x3c / 4])(obj4c, nullptr);

    int entry = GetTableEntryDwordByIndex(skinTarget, nullptr, 28);
    void* entryObj = reinterpret_cast<void*>(entry);
    ((void*(__fastcall*)(void*, void*, void*))CSF_AT(void**, entryObj, 0)[0x3c / 4])(entryObj, nullptr, matBytes);

    float* matPtr = CSF_AT(float*, matBytes, 0);
    float selfX = matPtr[12];
    float selfZ = matPtr[14];

    float* pos = GetObjectPositionVector(a2, nullptr);
    float posX = pos[0];
    float posY = pos[1];
    float posZ = pos[2];
    (void)posY;

    float* ang = GfxMatrix_GetEulerAngles(reinterpret_cast<float**>(matBytes), nullptr,
                                          reinterpret_cast<float*>(anglesBytes));
    float yaw = ang[1];
    float sinYaw = (float)std::sin((double)yaw);
    float cosYaw = (float)std::cos((double)yaw);

    dir[0] = posX - selfX;
    dir[1] = posZ - selfZ;
    float* norm = NormalizeVector2(dir, nullptr);
    float nx = norm[0];
    float nz = norm[1];

    float dot = nz * cosYaw + sinYaw * nx;
    double angleDeg = CalculateArcCosine(dot) * (double)g_RadToDeg;
    bool result = angleDeg > (double)g_RotationWrapStep;

    if (CSF_AT(uint8_t, matBytes, 4) != 0) {
        RwMatrixDestroy(reinterpret_cast<std::uint32_t*>(CSF_AT(float*, matBytes, 0)));
    }
    return result;
}

HOOK(0x004DA830, C_0083dd58_IsTargetOutsideFacingCone, "auto");
