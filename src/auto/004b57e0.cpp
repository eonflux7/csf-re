#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(double, __cdecl, AngleDiffNormalized, 0x004068E0, (float a1, float a2));
CSF_FUNC(double, __cdecl, MenuMpCharacterSelect_NormalizeAngle, 0x00406950, (float angleRadians));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, AiCoverAttackTasks_GetPositionData, 0x004A9CA0, (S_004abb90* self, void*));
CSF_FUNC(char, __fastcall, CoverAttack_CheckState_4ADAE0, 0x004ADAE0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(double, __fastcall, S_00526d40_GetVectorAngleRadians, 0x006AB930, (S_00526d40* self, void*));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_HudNegativeHalfPi, 0x0082FACC);
CSF_GLOBAL(float, g_TransitionHalfPi, 0x0082FAD0);
CSF_GLOBAL(float, g_SearchBoundsPadding, 0x00833E20);

static bool __fastcall S_004abb90_CheckSearchCandidate(
    S_004abb90* self, void* /*edx*/, int candidate)
{
    void* candidateHandle = reinterpret_cast<void*>(candidate);
    int linked = GetLinkedObjectValue_4DB5C0(candidateHandle, nullptr);
    uint8_t* object = linked ? reinterpret_cast<uint8_t*>(linked) - 4 : nullptr;

    if (CSF_AT(int, object, 0x608) == -1 &&
        CSF_AT(int, object, 0x60C) == -1)
        return true;

    linked = GetLinkedObjectValue_4DB5C0(candidateHandle, nullptr);
    object = linked ? reinterpret_cast<uint8_t*>(linked) - 4 : nullptr;
    const int stateIndex = CSF_AT(int, object, 0x630);
    uint8_t* coverAttack = object + 0x5DC;
    uint8_t* state = coverAttack + (stateIndex < 0 ? 0x34 : 0x4C);
    if (!CoverAttack_CheckState_4ADAE0(
            coverAttack, nullptr, reinterpret_cast<int>(state)))
        return true;

    linked = GetLinkedObjectValue_4DB5C0(candidateHandle, nullptr);
    uint8_t* candidateObject =
        linked ? reinterpret_cast<uint8_t*>(linked) - 4 : nullptr;

    float* searchPosition = reinterpret_cast<float*>(
        AiCoverAttackTasks_GetPositionData(
            reinterpret_cast<S_004abb90*>(
                reinterpret_cast<uint8_t*>(self) + 0x38),
            nullptr));

    alignas(8) uint8_t vectorBytes[0x20];
    S_00526d40& searchVector =
        *reinterpret_cast<S_00526d40*>(vectorBytes);
    S_00526d40& objectVector =
        *reinterpret_cast<S_00526d40*>(vectorBytes + 0x0C);

    const float searchZ = searchPosition[2] - CSF_AT(float, candidateObject, 0x654);
    const float searchY = searchPosition[1] - CSF_AT(float, candidateObject, 0x650);
    const float searchX = searchPosition[0] - CSF_AT(float, candidateObject, 0x64C);
    CSF_AT(float, vectorBytes, 0x00) = searchX;
    CSF_AT(float, vectorBytes, 0x04) = searchY;
    CSF_AT(float, vectorBytes, 0x08) = searchZ;
    const float searchDistanceSquared =
        searchX * searchX + searchY * searchY + searchZ * searchZ;

    if (!(searchDistanceSquared > g_SearchBoundsPadding))
        return false;
    if (!self->field_0018)
        return true;

    linked = GetLinkedObjectValue_4DB5C0(candidateHandle, nullptr);
    object = linked ? reinterpret_cast<uint8_t*>(linked) - 4 : nullptr;
    float* objectPosition =
        GetObjectPositionVector(CSF_AT(void*, object, 0x2C), nullptr);

    const float objectZ = objectPosition[2] - CSF_AT(float, candidateObject, 0x654);
    const float objectY = objectPosition[1] - CSF_AT(float, candidateObject, 0x650);
    const float objectX = objectPosition[0] - CSF_AT(float, candidateObject, 0x64C);
    CSF_AT(float, vectorBytes, 0x0C) = objectX;
    CSF_AT(float, vectorBytes, 0x10) = objectY;
    CSF_AT(float, vectorBytes, 0x14) = objectZ;
    const float objectDistanceSquared =
        objectX * objectX + objectY * objectY + objectZ * objectZ;

    float objectAngle = static_cast<float>(
        S_00526d40_GetVectorAngleRadians(&objectVector, nullptr));
    objectAngle = static_cast<float>(
        MenuMpCharacterSelect_NormalizeAngle(objectAngle));

    float searchAngle = static_cast<float>(
        S_00526d40_GetVectorAngleRadians(&searchVector, nullptr));
    searchAngle = static_cast<float>(
        MenuMpCharacterSelect_NormalizeAngle(searchAngle));

    const float angleDifference =
        static_cast<float>(AngleDiffNormalized(objectAngle, searchAngle));

    const uint32_t direction = self->field_0018;
    if (direction == 1)
    {
        if (!(searchDistanceSquared <
              self->field_0020 * self->field_0020 + objectDistanceSquared))
            return false;
        if (!(searchDistanceSquared >
              objectDistanceSquared - self->field_0020 * self->field_0020))
            return false;
        if (!(angleDifference > g_fZero))
            return false;
        return angleDifference < g_TransitionHalfPi;
    }

    if (direction == 2)
    {
        if (!(searchDistanceSquared <
              self->field_0020 * self->field_0020 + objectDistanceSquared))
            return false;
        if (!(searchDistanceSquared >
              objectDistanceSquared - self->field_0020 * self->field_0020))
            return false;
        return angleDifference < g_fZero &&
               angleDifference > g_HudNegativeHalfPi;
    }

    if (direction == 4)
    {
        if (!(searchDistanceSquared >
              self->field_0020 * self->field_0020 + objectDistanceSquared))
            return false;
    }
    else
    {
        if (direction != 8)
            return false;
        if (!(searchDistanceSquared <
              objectDistanceSquared - self->field_0020 * self->field_0020))
            return false;
    }

    return std::fabs(angleDifference) < g_TransitionHalfPi;
}

HOOK(0x004B57E0, S_004abb90_CheckSearchCandidate, "auto");
