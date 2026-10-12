#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(double, __fastcall, C_008342e8_ComputeResolvedObjectScalar, 0x0048F220, (C_008342e8* self, void*));
CSF_FUNC(int, __fastcall, CoverAttackTasks_GrowRecordArray, 0x004ADE60, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(int, __fastcall, GfxMatrix_SetScaleAndUpdate, 0x006AD9B0, (void* self, void* , const uint32_t* src));
CSF_FUNC(int, __fastcall, GfxMatrix_RotateAroundAxisY, 0x006AE1D0, (int* self, void* , float angle));

using DirectionAngles = float[3];

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_fReturnedConstant_0082E030, 0x0082E030);
CSF_GLOBAL(float, g_DegreesToRadians, 0x00835440);
CSF_GLOBAL(DirectionAngles, g_NegativeNegativeAttackAngles, 0x00899780);
CSF_GLOBAL(DirectionAngles, g_NegativePositiveAttackAngles, 0x0089978C);
CSF_GLOBAL(DirectionAngles, g_PositiveNegativeAttackAngles, 0x00899798);
CSF_GLOBAL(DirectionAngles, g_PositivePositiveAttackAngles, 0x008997A4);
CSF_GLOBAL(DirectionAngles, g_NegativeFirstAttackAngles, 0x008997B0);
CSF_GLOBAL(DirectionAngles, g_PositiveFirstAttackAngles, 0x008997BC);
CSF_GLOBAL(DirectionAngles, g_NegativeSecondAttackAngles, 0x008997C8);
CSF_GLOBAL(DirectionAngles, g_PositiveSecondAttackAngles, 0x008997D4);
CSF_GLOBAL(float, g_AxisZ, 0x008A7D84);

static void __fastcall AiCombat_BuildDirectionalAttackRecords(
    void* self, void* /*edx*/, int linkedObject, float firstDirection,
    float secondDirection)
{
    void* recordArray = static_cast<std::uint8_t*>(self) + 4;
    const std::uint32_t capacity = CSF_AT(std::uint32_t, self, 0x08);
    if (capacity < 3)
        CoverAttackTasks_GrowRecordArray(recordArray, nullptr, 3 - capacity);

    float* angles;
    float magnitude;
    if (firstDirection == g_fZero)
    {
        if (secondDirection == g_fZero)
            return;

        angles = !(secondDirection > g_fZero)
            ? g_NegativeSecondAttackAngles
            : g_PositiveSecondAttackAngles;
        magnitude = std::fabs(secondDirection);
    }
    else if (secondDirection == g_fZero)
    {
        angles = !(firstDirection > g_fZero)
            ? g_NegativeFirstAttackAngles
            : g_PositiveFirstAttackAngles;
        magnitude = std::fabs(firstDirection);
    }
    else
    {
        if (!(firstDirection > g_fZero))
        {
            angles = !(secondDirection > g_fZero)
                ? g_NegativeNegativeAttackAngles
                : g_NegativePositiveAttackAngles;
        }
        else
        {
            angles = !(secondDirection > g_fZero)
                ? g_PositiveNegativeAttackAngles
                : g_PositivePositiveAttackAngles;
        }
        magnitude = (std::fabs(secondDirection) + std::fabs(firstDirection))
            * g_HalfDurationScale;
    }

    alignas(8) std::uint8_t matrixWrapperBytes[0x0C];
    CSF_AT(float*, matrixWrapperBytes, 0x08) = angles;

    CSF_AT(std::uint32_t, self, 0x0C) = 0;
    CSF_AT(std::uint32_t, self, 0x14) = 0;

    int linkedValue = GetLinkedObjectValue_4DB5C0(
        reinterpret_cast<void*>(linkedObject), nullptr);
    void* owner = linkedValue
        ? reinterpret_cast<void*>(linkedValue - 4) : nullptr;
    void* position = GetObjectPositionVector(
        CSF_AT(void*, owner, 0x2C), nullptr);

    std::uint32_t record[7];
    const float positionY = CSF_AT(float, position, 0x04);
    record[0] = CSF_AT(std::uint32_t, position, 0x00);
    CSF_AT(float, matrixWrapperBytes, 0x00) =
        positionY + g_fReturnedConstant_0082E030;
    const std::uint32_t positionZ = CSF_AT(std::uint32_t, position, 0x08);
    CSF_AT(std::uint32_t, matrixWrapperBytes, 0x04) = positionZ;
    record[1] = CSF_AT(std::uint32_t, matrixWrapperBytes, 0x00);
    record[2] = positionZ;

    for (std::uint32_t i = 0; i < 3; ++i)
    {
        CSF_AT(std::uint32_t*, matrixWrapperBytes, 0x00) = RwMatrixCreate();
        CSF_AT(std::uint8_t, matrixWrapperBytes, 0x04) = 1;
        GfxMatrix_SetScaleAndUpdate(
            matrixWrapperBytes, nullptr,
            reinterpret_cast<const uint32_t*>(&g_AxisZ));

        linkedValue = GetLinkedObjectValue_4DB5C0(
            reinterpret_cast<void*>(linkedObject), nullptr);
        owner = linkedValue
            ? reinterpret_cast<void*>(linkedValue - 4) : nullptr;
        const double heading = C_008342e8_ComputeResolvedObjectScalar(
            CSF_AT(C_008342e8*, owner, 0x2C), nullptr);
        const float angle = static_cast<float>(
            (heading + CSF_AT(float*, matrixWrapperBytes, 0x08)[i])
            * g_DegreesToRadians);
        GfxMatrix_RotateAroundAxisY(
            reinterpret_cast<int*>(matrixWrapperBytes), nullptr, angle);

        std::uint32_t* matrix =
            CSF_AT(std::uint32_t*, matrixWrapperBytes, 0x00);
        record[3] = CSF_AT(std::uint32_t, matrix, 0x20);
        record[4] = CSF_AT(std::uint32_t, matrix, 0x24);
        record[5] = CSF_AT(std::uint32_t, matrix, 0x28);
        CSF_AT(float, record, 0x18) = magnitude;

        if (CSF_AT(std::uint32_t, self, 0x0C) >=
            CSF_AT(std::uint32_t, self, 0x08))
        {
            CoverAttackTasks_GrowRecordArray(
                recordArray, nullptr, CSF_AT(int, self, 0x10));
        }

        const std::uint32_t count = CSF_AT(std::uint32_t, self, 0x0C);
        std::uint8_t* destination =
            CSF_AT(std::uint8_t*, self, 0x04) + 0x1C * count;
        CSF_AT(std::uint32_t, self, 0x0C) = count + 1;
        std::memcpy(destination, record, 0x1C);

        if (CSF_AT(std::uint8_t, matrixWrapperBytes, 0x04))
            RwMatrixDestroy(CSF_AT(std::uint32_t*, matrixWrapperBytes, 0x00));
    }
}

HOOK(0x004B8510, AiCombat_BuildDirectionalAttackRecords, "auto");
