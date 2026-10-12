#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, RpPTankAtomicLock, 0x005821D0, (int a1, uint32_t* a2, unsigned int a3, int a4));
CSF_FUNC(int, __cdecl, RpPTankAtomicUnlock, 0x005823E0, (int a1));
CSF_FUNC(int, __cdecl, RwV3dTransformPoints, 0x00623040, (int a1, int a2, int a3, int a4));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(void*, __fastcall, GfxPtank_GetPlacementState, 0x006F3FE0, (void* self, void* edx, void* a2));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static char __stdcall GfxPTank_ReflectParticlesBehindPlacementPlane(int* tank)
{
    alignas(8) uint8_t placementBytes[0x28];
    alignas(8) uint8_t matrixBytes[0x14];
    alignas(8) uint8_t transformedBytes[0x38];
    alignas(8) uint8_t centerBytes[0xC];
    alignas(8) uint8_t lockBytes[0x8];

    GfxPtank_GetPlacementState(tank, nullptr, placementBytes);

    void* properties = CSF_AT(void*, placementBytes, 0x10);
    const bool matrixParticles = (CSF_AT(uint32_t, properties, 0x0) & 8) != 0;
    RpPTankAtomicLock(
        CSF_AT(int, properties, 0x18),
        reinterpret_cast<uint32_t*>(lockBytes),
        matrixParticles ? 8 : 1,
        0x40000000);

    void* placement = CSF_AT(void*, placementBytes, 0x0);
    CSF_AT(float, centerBytes, 0x0) = CSF_AT(float, placement, 0x1C);
    CSF_AT(float, centerBytes, 0x4) = CSF_AT(float, placement, 0x20);
    CSF_AT(float, centerBytes, 0x8) = CSF_AT(float, placement, 0x24);

    ((void(__fastcall*)(void*, void*, void*))CSF_AT(void**, tank, 0)[0x4C / 4])(
        tank, nullptr, matrixBytes);

    RwV3dTransformPoints(
        reinterpret_cast<int>(transformedBytes),
        reinterpret_cast<int>(centerBytes),
        1,
        CSF_AT(int, matrixBytes, 0x0));

    const float centerX = CSF_AT(float, transformedBytes, 0x0);
    const float centerY = CSF_AT(float, transformedBytes, 0x4);
    const float centerZ = CSF_AT(float, transformedBytes, 0x8);

    void* matrix = CSF_AT(void*, matrixBytes, 0x0);
    const float normalX = CSF_AT(float, matrix, 0x20);
    const float normalY = CSF_AT(float, matrix, 0x24);
    const float normalZ = CSF_AT(float, matrix, 0x28);

    const int particleCount =
        CSF_AT(int, CSF_AT(void*, placementBytes, 0x10), 0x8);

    bool planePending = true;
    float planeOffset = 0.0f;
    for (uint32_t index = 0; index < static_cast<uint32_t>(particleCount); ++index)
    {
        uint8_t* particle = CSF_AT(uint8_t*, lockBytes, 0x0);
        uint8_t* position = matrixParticles ? particle + 0x30 : particle;

        if (planePending)
        {
            planePending = false;
            planeOffset = -(normalZ * centerZ + normalY * centerY + normalX * centerX);
        }

        if (normalZ * CSF_AT(float, position, 0x8)
                + normalX * CSF_AT(float, position, 0x0)
                + normalY * CSF_AT(float, position, 0x4)
                + planeOffset < g_fZero)
        {
            const float deltaX = centerX - CSF_AT(float, position, 0x0);
            const float deltaY = centerY - CSF_AT(float, position, 0x4);
            const float deltaZ = centerZ - CSF_AT(float, position, 0x8);

            CSF_AT(float, position, 0x0) =
                deltaX + deltaX + CSF_AT(float, position, 0x0);
            CSF_AT(float, position, 0x4) =
                deltaY + deltaY + CSF_AT(float, position, 0x4);
            CSF_AT(float, position, 0x8) =
                deltaZ + deltaZ + CSF_AT(float, position, 0x8);

            particle = CSF_AT(uint8_t*, lockBytes, 0x0);
        }

        CSF_AT(uint8_t*, lockBytes, 0x0) =
            particle + CSF_AT(int, lockBytes, 0x4);
    }

    RpPTankAtomicUnlock(
        CSF_AT(int, CSF_AT(void*, placementBytes, 0x10), 0x18));

    const char ownsMatrix = CSF_AT(char, matrixBytes, 0x4);
    if (ownsMatrix)
        return static_cast<char>(
            RwMatrixDestroy(CSF_AT(std::uint32_t*, matrixBytes, 0x0)));
    return ownsMatrix;
}

HOOK(0x006D8710, GfxPTank_ReflectParticlesBehindPlacementPlane, "auto");
