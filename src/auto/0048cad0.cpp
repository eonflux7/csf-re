#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(double, __cdecl, AngleDiffNormalized, 0x004068E0, (float a1, float a2));
CSF_FUNC(double, __fastcall, AngleTable_GetAbsoluteAngleForIndex, 0x00406E10, (void* self, void* , int index));
CSF_FUNC(double, __fastcall, C_008342e8_ComputeResolvedObjectScalar, 0x0048F220, (C_008342e8* self, void*));
CSF_FUNC(double, __fastcall, S_00526d40_GetVectorAngleRadians, 0x006AB930, (S_00526d40* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_DegreesToRadians, 0x00833830);

static bool __fastcall S_0048cad0_CheckAngleConstraints(
    S_0048cad0* self, void* /*edx*/, char flags, float angleLimit)
{
    auto* owner = reinterpret_cast<C_008342e8*>(self->field_0004);
    void* skin = reinterpret_cast<void*>(owner->field_004c);
    void* angleTable =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, skin, 0)[0x3C / 4])(skin, nullptr);

    bool passed = true;
    if ((flags & 1) != 0)
    {
        alignas(8) uint8_t positionBytes[0x10];
        alignas(8) uint8_t directionBytes[0x0C];

        owner = reinterpret_cast<C_008342e8*>(self->field_0004);
        reinterpret_cast<void (__fastcall*)(void*, void*, float*)>(
            owner->vtbl[0xA4 / 4])(
                owner, nullptr, reinterpret_cast<float*>(positionBytes));

        const double headingDegrees = C_008342e8_ComputeResolvedObjectScalar(
            reinterpret_cast<C_008342e8*>(self->field_0004), nullptr);

        CSF_AT(float, directionBytes, 0x00) =
            self->field_0014 - CSF_AT(float, positionBytes, 0x00);
        CSF_AT(float, directionBytes, 0x04) =
            self->field_0018 - CSF_AT(float, positionBytes, 0x04);
        CSF_AT(float, directionBytes, 0x08) =
            self->field_001c - CSF_AT(float, positionBytes, 0x08);

        const float headingRadians =
            static_cast<float>(headingDegrees * g_DegreesToRadians);
        const float targetAngle = static_cast<float>(
            S_00526d40_GetVectorAngleRadians(
                reinterpret_cast<S_00526d40*>(directionBytes), nullptr));
        const double difference =
            AngleDiffNormalized(targetAngle, headingRadians);
        const float storedDifference = static_cast<float>(difference);

        double allowedAngle = difference;
        if (static_cast<int16_t>(self->field_0022) > 0)
        {
            void* settings = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
            allowedAngle =
                CSF_AT(float, settings, 0xAC0) * g_DegreesToRadians;
        }
        passed = std::fabs(storedDifference) <= allowedAngle;
    }

    if ((flags & 2) != 0)
        passed = passed &&
            AngleTable_GetAbsoluteAngleForIndex(angleTable, nullptr, 0) < angleLimit;

    if ((flags & 4) == 0)
        return passed;

    return passed &&
        AngleTable_GetAbsoluteAngleForIndex(angleTable, nullptr, 2) < angleLimit;
}

HOOK(0x0048CAD0, S_0048cad0_CheckAngleConstraints, "auto");
