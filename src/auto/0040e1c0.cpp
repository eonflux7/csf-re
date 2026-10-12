#include <cstdint>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void*));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GfxMatrix_CopyTranslationAndUpdate, 0x006AD9E0, (void* self, void* , const uint32_t* src));
CSF_FUNC(int, __fastcall, S_006ada10_SetRotation, 0x006AE2F0, (S_006ada10* self, void* , float angleX, float angleY, float angleZ));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());
CSF_FUNC(void*, __fastcall, C_0082dd78_DispatchReturnedObject, 0x0041E0C0, (void* self, void* edx));
CSF_FUNC(const uint32_t*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_MaximumHeadTimeStep, 0x0082DC68);
CSF_GLOBAL(float, g_RollOscillationScale, 0x0082DC6C);
CSF_GLOBAL(float, g_SubtitlePanelScale, 0x0082DC70);
CSF_GLOBAL(float, g_RadiansToDegrees, 0x0082DD58);
CSF_GLOBAL(float, g_DegreesToRadians, 0x0082DD5C);
CSF_GLOBAL(float, g_TurnLeanScale, 0x0082DD64);
CSF_GLOBAL(float, g_PreviewTopScale, 0x0082DD68);
CSF_GLOBAL(float, g_SpeedUnitScale, 0x0082DD6C);
CSF_GLOBAL(float, g_SpeedTimeScale, 0x0082DD70);

static float FloatFromBits(std::uint32_t bits)
{
    float value;
    std::memcpy(&value, &bits, sizeof(value));
    return value;
}

static std::uint32_t FloatToBits(float value)
{
    std::uint32_t bits;
    std::memcpy(&bits, &value, sizeof(bits));
    return bits;
}

static char __fastcall C_0082dd78_UpdateMotionRotation(
    C_0082dd78* self, void* /*edx*/, float* timeStep)
{
    alignas(8) std::uint8_t matrixBytes[0x14];
    S_006ada10& matrix = *reinterpret_cast<S_006ada10*>(matrixBytes);
    float& pitchOffset = CSF_AT(float, matrixBytes, 0x08);
    float& pitchPhase = CSF_AT(float, matrixBytes, 0x0C);
    float& rollOffset = CSF_AT(float, matrixBytes, 0x10);

    rollOffset =
        FloatFromBits(self->field_0064) * FloatFromBits(self->field_0064) +
        FloatFromBits(self->field_0068) * FloatFromBits(self->field_0068) +
        FloatFromBits(self->field_006c) * FloatFromBits(self->field_006c);

    void* speedSettings = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    pitchOffset = std::sqrt(rollOffset) /
        (CSF_AT(float, speedSettings, 0x1D70) *
         g_SpeedTimeScale * g_SpeedUnitScale) * g_PreviewTopScale;

    rollOffset =
        FloatFromBits(self->field_0064) * FloatFromBits(self->field_0064) +
        FloatFromBits(self->field_0068) * FloatFromBits(self->field_0068) +
        FloatFromBits(self->field_006c) * FloatFromBits(self->field_006c);

    speedSettings = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    void* turnSettings = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    rollOffset = std::sqrt(rollOffset) /
        (CSF_AT(float, speedSettings, 0x1D70) *
         g_SpeedTimeScale * g_SpeedUnitScale) *
        FloatFromBits(self->field_0058) /
        (CSF_AT(float, turnSettings, 0x1D74) * g_DegreesToRadians) *
        g_TurnLeanScale;

    int globalObject = GetGlobalValue_4FBDE0();
    int firstElement = GetFirstElement(reinterpret_cast<void*>(globalObject), nullptr);
    void* provider = GetCachedObjectProvider(reinterpret_cast<void*>(firstElement), nullptr);
    void* referencedObject = GetReferencedObject(provider, nullptr);
    void* leanController =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, referencedObject, 0)[0x10 / 4])(
                referencedObject, nullptr);
    float leanDegrees = rollOffset * g_RadiansToDegrees * g_HalfDurationScale;
    reinterpret_cast<void (__fastcall*)(void*, void*, float)>(
        CSF_AT(void**, leanController, 0)[0x38 / 4])(
            leanController, nullptr, leanDegrees);

    self->field_004c = FloatToBits(rollOffset);
    self->field_0048 = 0.0f;
    self->field_003c = FloatToBits(
        *timeStep * g_SubtitlePanelScale + FloatFromBits(self->field_003c));
    float rollPhase = FloatFromBits(self->field_003c);
    float delta = *timeStep;
    void* owner = reinterpret_cast<void*>(self->field_0004);
    pitchPhase = delta + delta + FloatFromBits(self->field_0040);
    self->field_0040 = FloatToBits(pitchPhase);

    void* transformOwner =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, owner, 0)[0x28 / 4])(owner, nullptr);

    matrix.field_0000 = reinterpret_cast<decltype(matrix.field_0000)>(RwMatrixCreate());
    float yaw = FloatFromBits(self->field_0044);
    CSF_AT(std::uint8_t, matrixBytes, 0x04) = 1;
    float roll = std::sin(rollPhase) * g_RollOscillationScale + rollOffset;
    float pitch = std::sin(pitchPhase) * g_MaximumHeadTimeStep + pitchOffset;
    S_006ada10_SetRotation(&matrix, nullptr, pitch, yaw, roll);

    const uint32_t* position = GetObjectPositionVector(
        reinterpret_cast<void*>(self->field_0004), nullptr);
    GfxMatrix_CopyTranslationAndUpdate(&matrix, nullptr, position);

    void* transform =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, transformOwner, 0)[0x18 / 4])(
                transformOwner, nullptr);
    reinterpret_cast<void (__fastcall*)(void*, void*, S_006ada10*)>(
        CSF_AT(void**, transform, 0)[0x94 / 4])(transform, nullptr, &matrix);

    void* updatedObject = C_0082dd78_DispatchReturnedObject(self, nullptr);
    reinterpret_cast<void (__fastcall*)(void*, void*)>(
        CSF_AT(void**, updatedObject, 0)[0x28 / 4])(updatedObject, nullptr);

    char ownsMatrix = static_cast<char>(CSF_AT(std::uint8_t, matrixBytes, 0x04));
    if (ownsMatrix)
        return static_cast<char>(RwMatrixDestroy(
            reinterpret_cast<std::uint32_t*>(matrix.field_0000)));
    return ownsMatrix;
}

HOOK(0x0040E1C0, C_0082dd78_UpdateMotionRotation, "auto");
