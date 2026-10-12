#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, RpPTankAtomicLock, 0x005821D0, (int a1, uint32_t* a2, unsigned int a3, int a4));
CSF_FUNC(int, __cdecl, RpPTankAtomicUnlock, 0x005823E0, (int a1));
CSF_FUNC(int, __cdecl, RwV3dTransformPoints, 0x00623040, (int a1, int a2, int a3, int a4));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(void*, __fastcall, GfxPTank_GetEmitterContext, 0x006F3FE0, (void* self, void* edx, void* a2));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static void __stdcall GfxPTank_WrapParticlePositions(void* emitter)
{
    alignas(8) uint8_t contextBytes[0x40];
    alignas(8) uint8_t positionBytes[0x70];
    alignas(8) uint8_t matrixResultBytes[0x8];
    alignas(8) uint8_t lockBytes[0x8];
    alignas(8) uint8_t transformedBytes[0xC];

    GfxPTank_GetEmitterContext(emitter, nullptr, contextBytes);

    void* tank = CSF_AT(void*, contextBytes, 0x10);
    const bool matrixParticles = (CSF_AT(uint32_t, tank, 0x0) & 8) != 0;
    RpPTankAtomicLock(
        CSF_AT(int, tank, 0x18),
        reinterpret_cast<uint32_t*>(lockBytes),
        matrixParticles ? 8 : 1,
        0x40000000);

    void* parameters = CSF_AT(void*, contextBytes, 0x0);
    CSF_AT(float, positionBytes, 0x0) = CSF_AT(float, parameters, 0x1C);
    CSF_AT(float, positionBytes, 0x4) = CSF_AT(float, parameters, 0x20);
    CSF_AT(float, positionBytes, 0x8) = CSF_AT(float, parameters, 0x24);

    ((void(__fastcall*)(void*, void*, void*))
        CSF_AT(void**, emitter, 0x0)[0x4C / 4])(
            emitter, nullptr, matrixResultBytes);

    RwV3dTransformPoints(
        reinterpret_cast<int>(transformedBytes),
        reinterpret_cast<int>(positionBytes),
        1,
        reinterpret_cast<int>(CSF_AT(void*, matrixResultBytes, 0x0)));

    const float centerX = CSF_AT(float, transformedBytes, 0x0);
    const float centerY = CSF_AT(float, transformedBytes, 0x4);
    const float centerZ = CSF_AT(float, transformedBytes, 0x8);

    void* matrix = CSF_AT(void*, matrixResultBytes, 0x0);
    const float forwardX = CSF_AT(float, matrix, 0x20);
    const float forwardY = CSF_AT(float, matrix, 0x24);
    const float forwardZ = CSF_AT(float, matrix, 0x28);
    const float rightX = CSF_AT(float, matrix, 0x0);
    const float rightY = CSF_AT(float, matrix, 0x4);
    const float rightZ = CSF_AT(float, matrix, 0x8);

    void* propertyObject = CSF_AT(void*, contextBytes, 0x1C);
    void* properties = CSF_AT(void*, propertyObject, 0x0);
    float forwardExtent;
    float rightExtent;
    if (CSF_AT(int, properties, 0xD4) != 0)
    {
        void* size = CSF_AT(void*, contextBytes, 0x24);
        forwardExtent = CSF_AT(float, size, 0x4) + CSF_AT(float, size, 0x0);
        rightExtent = forwardExtent;
    }
    else
    {
        parameters = CSF_AT(void*, contextBytes, 0x0);
        forwardExtent = CSF_AT(float, parameters, 0x28);
        rightExtent = CSF_AT(float, parameters, 0x30);
    }

    tank = CSF_AT(void*, contextBytes, 0x10);
    uint32_t remaining = CSF_AT(uint32_t, tank, 0x8);

    const float forwardWrapX = forwardX * (forwardExtent + forwardExtent);
    const float forwardWrapY = forwardY * (forwardExtent + forwardExtent);
    const float forwardWrapZ = forwardZ * (forwardExtent + forwardExtent);
    const float rightWrapX = rightX * (rightExtent + rightExtent);
    const float rightWrapY = rightY * (rightExtent + rightExtent);
    const float rightWrapZ = rightZ * (rightExtent + rightExtent);

    bool initializeForwardPlane = true;
    bool initializeRightPlane = true;
    float forwardPlaneOffset = 0;
    float rightPlaneOffset = 0;

    while (remaining != 0)
    {
        auto* position = reinterpret_cast<uint8_t*>(
            CSF_AT(uint32_t, lockBytes, 0x0));
        if (matrixParticles)
            position += 48;

        if (initializeForwardPlane)
        {
            forwardPlaneOffset = -(forwardZ * centerZ
                                 + forwardY * centerY
                                 + forwardX * centerX);
            initializeForwardPlane = false;
        }

        const float forwardDistance =
            forwardX * CSF_AT(float, position, 0x0)
          + forwardY * CSF_AT(float, position, 0x4)
          + forwardZ * CSF_AT(float, position, 0x8)
          + forwardPlaneOffset;

        if (std::fabs(forwardDistance) > forwardExtent)
        {
            const bool negative = forwardDistance < g_fZero;
            CSF_AT(float, position, 0x0) =
                (negative ? forwardWrapX : -forwardWrapX)
                + CSF_AT(float, position, 0x0);
            CSF_AT(float, position, 0x4) =
                (negative ? forwardWrapY : -forwardWrapY)
                + CSF_AT(float, position, 0x4);
            CSF_AT(float, position, 0x8) =
                (negative ? forwardWrapZ : -forwardWrapZ)
                + CSF_AT(float, position, 0x8);
        }

        if (initializeRightPlane)
        {
            rightPlaneOffset = -(rightZ * centerZ
                               + rightY * centerY
                               + rightX * centerX);
            initializeRightPlane = false;
        }

        const float rightDistance =
            rightX * CSF_AT(float, position, 0x0)
          + rightY * CSF_AT(float, position, 0x4)
          + rightZ * CSF_AT(float, position, 0x8)
          + rightPlaneOffset;

        if (std::fabs(rightDistance) > rightExtent)
        {
            const bool negative = rightDistance < g_fZero;
            CSF_AT(float, position, 0x0) =
                CSF_AT(float, position, 0x0)
                + (negative ? rightWrapX : -rightWrapX);
            CSF_AT(float, position, 0x4) =
                (negative ? rightWrapY : -rightWrapY)
                + CSF_AT(float, position, 0x4);
            CSF_AT(float, position, 0x8) =
                (negative ? rightWrapZ : -rightWrapZ)
                + CSF_AT(float, position, 0x8);
        }

        --remaining;
        CSF_AT(uint32_t, lockBytes, 0x0) += CSF_AT(uint32_t, lockBytes, 0x4);
    }

    tank = CSF_AT(void*, contextBytes, 0x10);
    RpPTankAtomicUnlock(CSF_AT(int, tank, 0x18));
    if (CSF_AT(uint8_t, matrixResultBytes, 0x4))
        RwMatrixDestroy(CSF_AT(std::uint32_t*, matrixResultBytes, 0x0));
}

HOOK(0x006D83F0, GfxPTank_WrapParticlePositions, "auto");
