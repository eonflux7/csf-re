#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(char, __fastcall, AiFncStatic_TestSegment, 0x004AF540, (void* self, void* edx, float* a2, uint32_t* a3));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(float*, __fastcall, NormalizeVector3, 0x006AB8D0, (C_00833b88* self, void*));
CSF_FUNC(int, __fastcall, GfxMatrix_SetScaleAndUpdate, 0x006AD9B0, (void* self, void* , const uint32_t* src));
CSF_FUNC(int, __fastcall, GfxMatrix_RotateAroundAxisY, 0x006AE1D0, (int* self, void* , float angle));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_fReturnedConstant_0082E030, 0x0082E030);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);
CSF_GLOBAL(float, g_ZeroVectorY, 0x008BB424);
CSF_GLOBAL(float, g_ZeroVectorZ, 0x008BB428);

static char __fastcall AiFncStatic_TryStoreLateralPosition(
    void* self, void* /*edx*/, int actor, float side)
{
    auto* guard = reinterpret_cast<C_008342e8*>(
        CHandleTable_Resolve(
            g_pCurrentScene, nullptr,
            static_cast<int>(reinterpret_cast<std::uintptr_t>(
                static_cast<std::uint8_t*>(self) + 0x14))));
    if (!guard)
        return 0;

    alignas(8) std::uint8_t positionBytes[0x18];
    auto* positions = reinterpret_cast<float*>(positionBytes);

    int linkedValue = GetLinkedObjectValue_4DB5C0(
        reinterpret_cast<void*>(actor), nullptr);
    void* linkedObject = linkedValue
        ? reinterpret_cast<void*>(linkedValue - 4)
        : nullptr;
    void* actorTransform = CSF_AT(void*, linkedObject, 0x2C);
    reinterpret_cast<float* (__fastcall*)(void*, void*, float*)>(
        CSF_AT(void**, actorTransform, 0)[0xA4 / 4])(
            actorTransform, nullptr, positions);

    float* position = GetObjectPositionVector(guard, nullptr);
    float savedX = position[0];
    float savedY = position[1];
    positions[5] = position[2];
    const std::uint32_t flags = guard->field_0170;
    positions[3] = savedX;
    positions[4] = savedY;

    alignas(8) std::uint8_t alternatePositionBytes[0xC];
    if ((flags & 0x2000) != 0)
    {
        void* guardObject = guard;
        float* alternatePosition =
            reinterpret_cast<float* (__fastcall*)(void*, void*, float*)>(
                CSF_AT(void**, guardObject, 0)[0xA4 / 4])(
                    guardObject, nullptr,
                    reinterpret_cast<float*>(alternatePositionBytes));
        savedX = alternatePosition[0];
        savedY = alternatePosition[1];
        positions[3] = savedX;
        positions[4] = savedY;
        positions[5] = alternatePosition[2];
    }

    alignas(8) std::uint8_t directionBytes[0x18];
    auto* direction = reinterpret_cast<float*>(directionBytes);
    direction[0] = positions[0] - positions[3];
    direction[1] = 0.0f;
    direction[2] = positions[2] - positions[5];
    NormalizeVector3(reinterpret_cast<C_00833b88*>(directionBytes), nullptr);

    alignas(8) std::uint8_t matrixBytes[0x10] = {};
    CSF_AT(std::uint32_t*, matrixBytes, 0) = RwMatrixCreate();
    CSF_AT(std::uint8_t, matrixBytes, 4) = 1;
    GfxMatrix_SetScaleAndUpdate(
        matrixBytes, nullptr,
        reinterpret_cast<const std::uint32_t*>(directionBytes));
    GfxMatrix_RotateAroundAxisY(
        reinterpret_cast<int*>(matrixBytes), nullptr,
        side > g_fZero ? 1.5707964f : -1.5707964f);

    alignas(8) std::uint8_t segmentBytes[0x18];
    auto* segment = reinterpret_cast<float*>(segmentBytes);
    segment[0] = g_ZeroVectorX;
    segment[3] = segment[0];
    segment[1] = g_ZeroVectorY;
    segment[4] = segment[1];
    segment[2] = g_ZeroVectorZ;
    segment[5] = segment[2];

    auto* matrix = reinterpret_cast<float*>(
        CSF_AT(std::uint32_t*, matrixBytes, 0));
    const float lateralX = matrix[8] * g_fReturnedConstant_0082E030;
    const float lateralY = matrix[9] * g_fReturnedConstant_0082E030;
    const float lateralZ = matrix[10] * g_fReturnedConstant_0082E030;
    segment[1] = positions[1];
    segment[0] = positions[0];
    segment[2] = positions[2];

    alignas(8) std::uint8_t resultBytes[0xC] = {};
    direction[3] = lateralX + positions[3];
    segment[3] = direction[3];
    direction[4] = lateralY + positions[4];
    segment[4] = direction[4];
    direction[5] = lateralZ + positions[5];
    segment[5] = direction[5];

    linkedValue = GetLinkedObjectValue_4DB5C0(
        reinterpret_cast<void*>(actor), nullptr);
    linkedObject = linkedValue
        ? reinterpret_cast<void*>(linkedValue - 4)
        : nullptr;

    char success = 0;
    if (!AiFncStatic_TestSegment(
            linkedObject, nullptr, segment,
            reinterpret_cast<std::uint32_t*>(resultBytes)))
    {
        CSF_AT(float, self, 0x20) = savedX;
        CSF_AT(float, self, 0x24) = savedY;
        CSF_AT(float, self, 0x28) = positions[5];
        success = 1;
    }

    if (CSF_AT(std::uint8_t, matrixBytes, 4))
        RwMatrixDestroy(CSF_AT(std::uint32_t*, matrixBytes, 0));

    return success;
}

HOOK(0x004B0B30, AiFncStatic_TryStoreLateralPosition, "auto");
