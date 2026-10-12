#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, RpPrtStdPropTabGetPropOffset, 0x0057C8B0, (int a1, int a2));
CSF_FUNC(int, __cdecl, RpPTankAtomicLock, 0x005821D0, (int a1, uint32_t* a2, unsigned int a3, int a4));
CSF_FUNC(int, __cdecl, RpPTankAtomicUnlock, 0x005823E0, (int a1));
CSF_FUNC(double, __cdecl, RwV3dLength, 0x00623360, (float* a1));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(int, __fastcall, GfxMatrix_CopyVec3AndUpdate, 0x006AD950, (uint32_t* self, void* , uint32_t* src));
CSF_FUNC(int, __fastcall, GfxMatrix_SetScaleAndUpdate, 0x006AD9B0, (void* self, void* , const uint32_t* src));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());

CSF_GLOBAL(float, g_MissingMetricValue, 0x0082D4C4);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_ColorChannelScale, 0x0082FEA8);
CSF_GLOBAL(float, g_ParticleFacingEpsilon, 0x0086319C);

static int __cdecl GfxParticleRender_UpdateFacingAndAlpha(int /*atomic*/, int emitterAddress)
{
    auto* viewPosition = reinterpret_cast<float*>(
        static_cast<std::uintptr_t>(static_cast<std::uint32_t>(GetGlobalValue_6C5760())) + 0x124);
    auto* emitter = reinterpret_cast<std::uint8_t*>(
        static_cast<std::uintptr_t>(static_cast<std::uint32_t>(emitterAddress)));

    int facingOffset = RpPrtStdPropTabGetPropOffset(
        CSF_AT(int, CSF_AT(void*, emitter, 0xC), 0x10), 10);
    int tankOffset = RpPrtStdPropTabGetPropOffset(
        CSF_AT(int, CSF_AT(void*, emitter, 0xC), 0x10), 6);

    auto* facing = facingOffset == -1 ? nullptr : emitter + facingOffset;
    auto* tank = tankOffset == -1 ? nullptr : emitter + tankOffset;

    int facingMode = CSF_AT(int, CSF_AT(void*, facing, 0), 0x100);
    int alphaLimit = static_cast<int>(
        static_cast<std::int64_t>(CSF_AT(float, facing, 4) * g_ColorChannelScale));
    bool limitAlpha = CSF_AT(float, facing, 4) < g_TransitionOne;

    alignas(8) std::uint8_t matrixLockBytes[0x8];
    alignas(8) std::uint8_t colorLockBytes[0x8];
    auto* matrixLock = reinterpret_cast<std::uint32_t*>(matrixLockBytes);
    auto* colorLock = reinterpret_cast<std::uint32_t*>(colorLockBytes);

    RpPTankAtomicLock(CSF_AT(int, tank, 0x18), matrixLock, 8, 0x40000000);
    RpPTankAtomicLock(CSF_AT(int, tank, 0x18), colorLock, 2, 0x40000000);

    std::uint32_t particleIndex = 0;
    if (CSF_AT(std::uint32_t, tank, 8) != 0)
    {
        float* matrix = CSF_AT(float*, matrixLockBytes, 0);
        do
        {
            if (facingMode == 1)
            {
                alignas(8) std::uint8_t matrixRefBytes[0x8];
                CSF_AT(float*, matrixRefBytes, 0) = matrix;
                CSF_AT(std::uint8_t, matrixRefBytes, 4) = 0;

                alignas(8) std::uint8_t rightBytes[0x10];
                alignas(8) std::uint8_t forwardBytes[0x24];
                auto* right = reinterpret_cast<float*>(rightBytes);
                auto* forward = reinterpret_cast<float*>(forwardBytes);

                float dx = viewPosition[0] - matrix[12];
                float dy = viewPosition[1] - matrix[13];
                float dz = viewPosition[2] - matrix[14];
                right[0] = dy * matrix[6] - dz * matrix[5];
                right[1] = dz * matrix[4] - dx * matrix[6];
                right[2] = dx * matrix[5] - dy * matrix[4];

                double rightLength = RwV3dLength(right);
                float storedRightLength = static_cast<float>(rightLength);
                if (rightLength > g_ParticleFacingEpsilon)
                {
                    double originalLength = RwV3dLength(CSF_AT(float*, matrixRefBytes, 0));
                    double scale = originalLength / storedRightLength;
                    matrix = CSF_AT(float*, matrixRefBytes, 0);
                    right[0] = static_cast<float>(right[0] * scale);
                    right[1] = static_cast<float>(right[1] * scale);
                    right[2] = static_cast<float>(right[2] * scale);

                    forward[0] = right[1] * matrix[6] - right[2] * matrix[5];
                    forward[1] = right[2] * matrix[4] - right[0] * matrix[6];
                    forward[2] = right[0] * matrix[5] - right[1] * matrix[4];

                    GfxMatrix_CopyVec3AndUpdate(
                        reinterpret_cast<std::uint32_t*>(matrixRefBytes), nullptr,
                        reinterpret_cast<std::uint32_t*>(rightBytes));
                    GfxMatrix_SetScaleAndUpdate(
                        matrixRefBytes, nullptr,
                        reinterpret_cast<const std::uint32_t*>(forwardBytes));
                }
                if (CSF_AT(std::uint8_t, matrixRefBytes, 4))
                    RwMatrixDestroy(CSF_AT(std::uint32_t*, matrixRefBytes, 0));
                matrix = CSF_AT(float*, matrixLockBytes, 0);
            }
            else if (facingMode == 2)
            {
                alignas(8) std::uint8_t matrixRefBytes[0x8];
                CSF_AT(float*, matrixRefBytes, 0) = matrix;
                CSF_AT(std::uint8_t, matrixRefBytes, 4) = 0;

                float dot = (viewPosition[1] - matrix[13]) * matrix[9]
                          + (viewPosition[0] - matrix[12]) * matrix[8]
                          + (viewPosition[2] - matrix[14]) * matrix[10];
                if (dot < g_fZero)
                {
                    alignas(8) std::uint8_t rightBytes[0xC];
                    alignas(8) std::uint8_t forwardBytes[0xC];
                    auto* right = reinterpret_cast<float*>(rightBytes);
                    auto* forward = reinterpret_cast<float*>(forwardBytes);

                    right[0] = matrix[0] * g_MissingMetricValue;
                    right[1] = matrix[1] * g_MissingMetricValue;
                    right[2] = matrix[2] * g_MissingMetricValue;
                    GfxMatrix_CopyVec3AndUpdate(
                        reinterpret_cast<std::uint32_t*>(matrixRefBytes), nullptr,
                        reinterpret_cast<std::uint32_t*>(rightBytes));

                    matrix = CSF_AT(float*, matrixRefBytes, 0);
                    forward[0] = matrix[8] * g_MissingMetricValue;
                    forward[1] = matrix[9] * g_MissingMetricValue;
                    forward[2] = matrix[10] * g_MissingMetricValue;
                    GfxMatrix_SetScaleAndUpdate(
                        matrixRefBytes, nullptr,
                        reinterpret_cast<const std::uint32_t*>(forwardBytes));

                    if (CSF_AT(std::uint8_t, matrixRefBytes, 4))
                        RwMatrixDestroy(CSF_AT(std::uint32_t*, matrixRefBytes, 0));
                    matrix = CSF_AT(float*, matrixLockBytes, 0);
                }
            }

            if (limitAlpha &&
                CSF_AT(std::uint8_t, CSF_AT(void*, colorLockBytes, 0), 3) > alphaLimit)
            {
                CSF_AT(std::uint8_t, CSF_AT(void*, colorLockBytes, 0), 3) =
                    static_cast<std::uint8_t>(alphaLimit);
                matrix = CSF_AT(float*, matrixLockBytes, 0);
            }

            colorLock[0] += colorLock[1];
            matrix = reinterpret_cast<float*>(
                reinterpret_cast<std::uint8_t*>(matrix) + matrixLock[1]);
            CSF_AT(float*, matrixLockBytes, 0) = matrix;
            ++particleIndex;
        }
        while (particleIndex < CSF_AT(std::uint32_t, tank, 8));
    }

    RpPTankAtomicUnlock(CSF_AT(int, tank, 0x18));
    return emitterAddress;
}

HOOK(0x00703D20, GfxParticleRender_UpdateFacingAndAlpha, "auto");
