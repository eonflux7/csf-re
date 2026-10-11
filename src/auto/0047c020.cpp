#include <cstdint>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(double, __cdecl, AngleDiffNormalized, 0x004068E0, (float a1, float a2));
CSF_FUNC(double, __fastcall, GetEntryFloatOrRadiansFromSource_406D70, 0x00406D70, (void* self, void* , int index));
CSF_FUNC(double, __fastcall, GetEntryFloatOrRadiansFromSource_406DC0, 0x00406DC0, (void* self, void* , int index));
CSF_FUNC(char, __fastcall, C_00862490_GetDirection, 0x00476100, (C_00862490* self, void* , std::uint32_t* direction));
CSF_FUNC(double, __fastcall, S_00526d40_GetVectorAngleRadians, 0x006AB930, (S_00526d40* self, void*));
CSF_FUNC(double, __fastcall, S_00526d40_NormalizeCopyAndReportY, 0x006AB9E0, (S_00526d40* self, void*));

CSF_GLOBAL(float, g_DefaultAngleLimit, 0x0082D7BC);

static char __fastcall C_00834fa8_IsAimAligned(C_00834fa8* self, void* /*edx*/)
{
    void* taskOwner = CSF_AT(void*, self, 0x08);
    void* components = CSF_AT(void*, taskOwner, 0x14);
    void* directionProvider = CSF_AT(void*, components, 0x44);
    C_00862490* directionObject =
        reinterpret_cast<C_00862490* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, directionProvider, 0)[0xD8 / 4])(
                directionProvider, nullptr);

    if (!directionObject)
        return 0;

    taskOwner = CSF_AT(void*, self, 0x08);
    components = CSF_AT(void*, taskOwner, 0x14);
    void* angleProvider = CSF_AT(void*, components, 0x4C);
    void* angleSource =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, angleProvider, 0)[0x3C / 4])(
                angleProvider, nullptr);

    alignas(8) std::uint8_t directionBytes[0x10];
    S_00526d40& direction =
        *reinterpret_cast<S_00526d40*>(directionBytes);
    std::memset(directionBytes, 0, 0x0C);
    C_00862490_GetDirection(
        directionObject, nullptr,
        reinterpret_cast<std::uint32_t*>(directionBytes));

    float sourcePitch = static_cast<float>(
        GetEntryFloatOrRadiansFromSource_406D70(angleSource, nullptr, 0));
    float directionPitch = static_cast<float>(
        -S_00526d40_NormalizeCopyAndReportY(&direction, nullptr));
    double pitchDifference = AngleDiffNormalized(directionPitch, sourcePitch);
    if (!(std::fabs(pitchDifference) < g_DefaultAngleLimit))
        return 0;

    float sourceHeading = static_cast<float>(
        GetEntryFloatOrRadiansFromSource_406DC0(angleSource, nullptr, 0));
    float directionHeading = static_cast<float>(
        S_00526d40_GetVectorAngleRadians(&direction, nullptr));
    double headingDifference = AngleDiffNormalized(directionHeading, sourceHeading);
    if (!(std::fabs(headingDifference) < g_DefaultAngleLimit))
        return 0;

    return 1;
}

HOOK(0x0047C020, C_00834fa8_IsAimAligned, "auto");
