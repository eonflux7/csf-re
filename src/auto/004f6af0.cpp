#include <cstdint>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, ClampFloatToBounds, 0x00406B60, (float* value, float* lowerBound, float* upperBound));
CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(double, __stdcall, CharacterModel_GetAngleDifference, 0x004F4E10, (float a1, float a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void*));
CSF_FUNC(uint32_t*, __cdecl, RwMatrixTransform, 0x00624C70, (uint32_t* a1, uint32_t* a2, int a3));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(int, __fastcall, GfxMatrix_Translate, 0x006ADBB0, (void* self, void* , int a2, int a3, int a4));
CSF_FUNC(int, __fastcall, GfxMatrix_RotateAroundAxisX, 0x006AE190, (int* self, void* , float angle));
CSF_FUNC(int, __fastcall, GfxMatrix_SetYRotation, 0x006AE2A0, (S_006ada10* self, void* , float angle));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_MinimumHeadTimeStep, 0x0082D4F0);
CSF_GLOBAL(float, g_MaximumHeadTimeStep, 0x0082DC68);
CSF_GLOBAL(float, g_HeadDampingScale, 0x0082DC84);
CSF_GLOBAL(float, g_HeadTranslationScale, 0x0082DFCC);
CSF_GLOBAL(float, g_NegativeWheelScale, 0x0082E250);
CSF_GLOBAL(float, g_PositiveHalfRotationDegrees, 0x00830D08);
CSF_GLOBAL(float, g_HeadDegreesToRadians, 0x0083F184);
CSF_GLOBAL(float, g_HeadSpringScale, 0x0083F198);

static char __fastcall CharacterModel_UpdateHeadTransform(
    float* self, void* /*edx*/, std::uint8_t* model, float* angles, float timeStep)
{
    reinterpret_cast<void (__fastcall*)(void*, void*)>(
        CSF_AT(void**, model, 0)[0x28 / 4])(model, nullptr);

    if (timeStep < g_MinimumHeadTimeStep)
        timeStep = 0.001f;
    else if (timeStep > g_MaximumHeadTimeStep)
        timeStep = 0.029999999f;

    double difference = CharacterModel_GetAngleDifference(
        angles[0], CSF_AT(float, self, 0x9C));
    CSF_AT(float, self, 0x8C) =
        static_cast<float>(difference + CSF_AT(float, self, 0x8C));

    difference = CharacterModel_GetAngleDifference(
        angles[1], CSF_AT(float, self, 0xA0));
    CSF_AT(float, self, 0x90) =
        static_cast<float>(difference + CSF_AT(float, self, 0x90));

    float upperYaw = 180.0f;
    float upperPitch = 180.0f;
    float lowerBound = -180.0f;
    ClampFloatToBounds(&CSF_AT(float, self, 0x8C), &lowerBound, &upperYaw);
    lowerBound = -180.0f;
    ClampFloatToBounds(&CSF_AT(float, self, 0x90), &lowerBound, &upperPitch);

    float yawAcceleration = CSF_AT(float, self, 0x8C) * g_HeadSpringScale;
    float pitchAcceleration = CSF_AT(float, self, 0x90) * g_HeadSpringScale;
    yawAcceleration *= CSF_AT(float,
        reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0x9A0);
    pitchAcceleration *= CSF_AT(float,
        reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0x9A0);

    CSF_AT(float, self, 0x94) += yawAcceleration * timeStep;
    CSF_AT(float, self, 0x98) += pitchAcceleration * timeStep;

    const float damping = CSF_AT(float,
        reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0x9AC) * g_HeadDampingScale;

    const bool yawWasPositive = CSF_AT(float, self, 0x94) > g_fZero;
    CSF_AT(float, self, 0x94) -=
        (g_PositiveHalfRotationDegrees - std::fabs(CSF_AT(float, self, 0x8C))) *
        CSF_AT(float, self, 0x94) * damping * timeStep;
    if (yawWasPositive ? CSF_AT(float, self, 0x94) < g_fZero
                       : CSF_AT(float, self, 0x94) > g_fZero)
        CSF_AT(float, self, 0x94) = 0.0f;

    const bool pitchWasPositive = CSF_AT(float, self, 0x98) > g_fZero;
    CSF_AT(float, self, 0x98) -=
        damping * ((g_PositiveHalfRotationDegrees -
                    std::fabs(CSF_AT(float, self, 0x90))) *
                   CSF_AT(float, self, 0x98)) * timeStep;
    if (pitchWasPositive ? CSF_AT(float, self, 0x98) < g_fZero
                         : CSF_AT(float, self, 0x98) > g_fZero)
        CSF_AT(float, self, 0x98) = 0.0f;

    CSF_AT(float, self, 0x8C) += timeStep * CSF_AT(float, self, 0x94);
    CSF_AT(float, self, 0x90) += timeStep * CSF_AT(float, self, 0x98);

    float translation = angles[1] * g_HeadTranslationScale;
    translation *= CSF_AT(float,
        reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0x9A8);
    float pitch = CSF_AT(float, self, 0x90);
    float yaw = CSF_AT(float, self, 0x8C);

    if ((CSF_AT(std::uint8_t, model, 0x2E0) & 0x40) != 0)
    {
        translation = 0.0f;
        pitch *= CSF_AT(float,
            reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0x9A4);
        yaw *= CSF_AT(float,
            reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0x9A4);
    }

    const float yawLimit = CSF_AT(float,
        reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0x998);
    const float pitchLimit = CSF_AT(float,
        reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0x99C);

    alignas(8) std::uint8_t rotationBytes[0x8];
    S_006ada10& rotation = *reinterpret_cast<S_006ada10*>(rotationBytes);
    std::uint32_t* matrix = RwMatrixCreate();
    std::memcpy(&rotation.field_0000, &matrix, sizeof(matrix));
    CSF_AT(std::uint8_t, rotationBytes, 0x4) = 1;

    const double yawExponential = std::exp2(
        yaw * g_NegativeWheelScale * 1.442695040888963407);
    const float yawRotation = static_cast<float>(
        (g_TransitionOne / (yawExponential + g_TransitionOne) *
         yawLimit * 2.0 - yawLimit) * g_HeadDegreesToRadians);
    GfxMatrix_SetYRotation(&rotation, nullptr, yawRotation);

    const double pitchExponential = std::exp2(
        pitch * g_NegativeWheelScale * 1.442695040888963407);
    const float pitchRotation = static_cast<float>(
        (g_TransitionOne / (pitchExponential + g_TransitionOne) *
         pitchLimit * 2.0 - pitchLimit) * g_HeadDegreesToRadians);
    GfxMatrix_RotateAroundAxisX(
        reinterpret_cast<int*>(rotationBytes), nullptr, pitchRotation);

    alignas(8) std::uint8_t transformBytes[0xC];
    CSF_AT(std::uint32_t*, transformBytes, 0) = RwMatrixCreate();
    CSF_AT(std::uint8_t, transformBytes, 0x4) = 1;

    int translationBits;
    std::memcpy(&translationBits, &translation, sizeof(translationBits));
    GfxMatrix_Translate(transformBytes, nullptr, 0, translationBits, -1027080192);
    std::memcpy(&matrix, &rotation.field_0000, sizeof(matrix));
    RwMatrixTransform(CSF_AT(std::uint32_t*, transformBytes, 0), matrix, 2);
    GfxMatrix_Translate(transformBytes, nullptr, 0, 0, 1120403456);

    const int collection = GetGlobalValue_4FBDE0();
    const int firstElement = GetFirstElement(reinterpret_cast<void*>(collection), nullptr);
    void* entry = reinterpret_cast<void*>(
        ResolveEntryLookupValue(reinterpret_cast<void*>(firstElement), nullptr));
    reinterpret_cast<void (__fastcall*)(void*, void*, int*)>(
        CSF_AT(void**, entry, 0)[0x54 / 4])(
            entry, nullptr, reinterpret_cast<int*>(transformBytes));

    CSF_AT(float, self, 0x9C) = angles[0];
    CSF_AT(float, self, 0xA0) = angles[1];

    if (CSF_AT(std::uint8_t, transformBytes, 0x4))
        RwMatrixDestroy(CSF_AT(std::uint32_t*, transformBytes, 0));

    const char ownsRotation = CSF_AT(char, rotationBytes, 0x4);
    if (ownsRotation)
    {
        std::memcpy(&matrix, &rotation.field_0000, sizeof(matrix));
        return static_cast<char>(RwMatrixDestroy(matrix));
    }
    return ownsRotation;
}

HOOK(0x004F6AF0, CharacterModel_UpdateHeadTransform, "auto");
