#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, AiStatic_TraceSegment, 0x004AF540, (void* self, void* edx, float* a2, uint32_t* a3));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(int, __fastcall, GfxMatrix_SetScaleAndUpdate, 0x006AD9B0, (void* self, void* , const uint32_t* src));
CSF_FUNC(int, __fastcall, GfxMatrix_RotateAroundAxisY, 0x006AE1D0, (int* self, void* , float angle));

CSF_GLOBAL(float, g_AxisZ, 0x008A7D84);
CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);
CSF_GLOBAL(float, g_ZeroVectorY, 0x008BB424);
CSF_GLOBAL(float, g_ZeroVectorZ, 0x008BB428);

static double __fastcall AiStatic_CalculateObstacleDistance(
    void* self, void* /*edx*/, void* linkedObject, float angle, float distance)
{
    alignas(8) std::uint8_t segmentBytes[0x30];
    alignas(8) std::uint8_t hitBytes[0xC];
    alignas(8) std::uint8_t positionBytes[0xC];
    alignas(8) std::uint8_t matrixBytes[0x8];

    float* segment = reinterpret_cast<float*>(segmentBytes);
    float* hit = reinterpret_cast<float*>(hitBytes);
    float* position = reinterpret_cast<float*>(positionBytes);

    int linkedValue = GetLinkedObjectValue_4DB5C0(linkedObject, nullptr);
    void* owner = linkedValue
        ? reinterpret_cast<void*>(linkedValue - 4)
        : nullptr;
    void* transform = CSF_AT(void*, owner, 0x2C);

    reinterpret_cast<void(__fastcall*)(void*, void*, float*)>(
        CSF_AT(void**, transform, 0)[0xA4 / 4])(
            transform, nullptr, position);

    CSF_AT(std::uint32_t*, matrixBytes, 0) = RwMatrixCreate();
    CSF_AT(std::uint8_t, matrixBytes, 4) = 1;
    GfxMatrix_SetScaleAndUpdate(
        matrixBytes, nullptr, reinterpret_cast<const uint32_t*>(&g_AxisZ));
    GfxMatrix_RotateAroundAxisY(
        reinterpret_cast<int*>(matrixBytes), nullptr, angle);

    segment[0] = segment[3] = g_ZeroVectorX;
    segment[1] = segment[4] = g_ZeroVectorY;
    segment[2] = segment[5] = g_ZeroVectorZ;
    hit[0] = hit[1] = hit[2] = 0.0f;

    std::uint32_t* matrix = CSF_AT(std::uint32_t*, matrixBytes, 0);
    float offsetX = distance * CSF_AT(float, matrix, 0x20);
    float offsetY = distance * CSF_AT(float, matrix, 0x24);
    float offsetZ = distance * CSF_AT(float, matrix, 0x28);

    segment[0] = position[0];
    segment[1] = position[1];
    segment[2] = position[2];
    segment[3] = position[0] + offsetX;
    segment[4] = position[1] + offsetY;
    segment[5] = position[2] + offsetZ;

    linkedValue = GetLinkedObjectValue_4DB5C0(linkedObject, nullptr);
    owner = linkedValue ? reinterpret_cast<void*>(linkedValue - 4) : nullptr;

    float result;
    if (AiStatic_TraceSegment(
            owner, nullptr, segment, reinterpret_cast<uint32_t*>(hitBytes)))
    {
        float dx = hit[0] - position[0];
        float dy = hit[1] - position[1];
        float dz = hit[2] - position[2];
        result = -std::sqrt(dz * dz + dy * dy + dx * dx);
    }
    else
    {
        result = CSF_AT(float, self, 0x1C);
    }

    if (CSF_AT(std::uint8_t, matrixBytes, 4))
        RwMatrixDestroy(CSF_AT(std::uint32_t*, matrixBytes, 0));

    return result;
}

HOOK(0x004B0860, AiStatic_CalculateObstacleDistance, "auto");
