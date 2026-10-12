#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, TargetPoint_SetAngles, 0x00406C90,
         (void* self, void* edx, int a2, uint32_t* a3));
CSF_FUNC(int, __fastcall, TargetPoint_ClearMode, 0x00406D40,
         (void* self, void* edx, int a2));
CSF_FUNC(double, __fastcall, S_00526d40_GetVectorAngleRadians, 0x006AB930,
         (S_00526d40* self, void*));
CSF_FUNC(double, __fastcall, S_00526d40_NormalizeCopyAndReportY, 0x006AB9E0,
         (S_00526d40* self, void*));

CSF_GLOBAL(float, g_TargetPointDistanceEpsilon, 0x0082DEB0);

static void __fastcall ApplyTargetPointRequest(
    void* self, void* /*edx*/, void* request)
{
    int mode = -1;
    const int requestedMode = CSF_AT(int, request, 0x04);
    if (requestedMode == 0)
        mode = 0;
    else if (requestedMode == 1)
        mode = 2;

    if (CSF_AT(uint8_t, request, 0x00) == 0)
    {
        TargetPoint_ClearMode(CSF_AT(void*, self, 0x10), nullptr, mode);
        return;
    }

    float position[3];
    void* positionProvider = CSF_AT(void*, self, 0x04);
    ((void(__fastcall*)(void*, void*, float*))
        CSF_AT(void**, positionProvider, 0x00)[0xA4 / 4])(
            positionProvider, nullptr, position);

    float displacement[3];
    displacement[0] = CSF_AT(float, request, 0x08) - position[0];
    displacement[1] = CSF_AT(float, request, 0x0C) - position[1];
    displacement[2] = CSF_AT(float, request, 0x10) - position[2];

    const float distance = std::sqrt(
        displacement[2] * displacement[2] +
        displacement[1] * displacement[1] +
        displacement[0] * displacement[0]);

    if (distance > g_TargetPointDistanceEpsilon)
    {
        float angles[3] = {0.0f, 0.0f, 0.0f};
        auto* direction = reinterpret_cast<S_00526d40*>(displacement);

        angles[0] = static_cast<float>(
            -S_00526d40_NormalizeCopyAndReportY(direction, nullptr));
        angles[1] = static_cast<float>(
            S_00526d40_GetVectorAngleRadians(direction, nullptr));

        TargetPoint_SetAngles(
            CSF_AT(void*, self, 0x10), nullptr, mode,
            reinterpret_cast<uint32_t*>(angles));
    }

}

HOOK(0x0040FC50, ApplyTargetPointRequest, "auto");
