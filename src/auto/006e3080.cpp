#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, RpLightGetWorld, 0x0058DFD0, (int a1));
CSF_FUNC(int, __cdecl, RwIm3DTransform, 0x0063CED0, (int a1, unsigned int a2, int a3, int a4));
CSF_FUNC(int, __cdecl, RwIm3DEnd, 0x0063CFA0, ());

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_LightBoundsExtentScale, 0x00861A44);

static float* __stdcall GfxIm3D_TransformLightWorldBounds(
    int light, std::uint8_t* color)
{
    float* world = reinterpret_cast<float*>(RpLightGetWorld(light));
    if (!world)
        return world;

    const float centerX =
        (CSF_AT(float, world, 0x5C) + CSF_AT(float, world, 0x50)) *
        g_HalfDurationScale;
    const float centerY =
        (CSF_AT(float, world, 0x60) + CSF_AT(float, world, 0x54)) *
        g_HalfDurationScale;
    const float centerZ =
        (CSF_AT(float, world, 0x64) + CSF_AT(float, world, 0x58)) *
        g_HalfDurationScale;

    const float extentX =
        (CSF_AT(float, world, 0x50) - CSF_AT(float, world, 0x5C)) *
        g_LightBoundsExtentScale;
    const float extentY =
        (CSF_AT(float, world, 0x54) - CSF_AT(float, world, 0x60)) *
        g_LightBoundsExtentScale;
    const float extentZ =
        (CSF_AT(float, world, 0x58) - CSF_AT(float, world, 0x64)) *
        g_LightBoundsExtentScale;

    const std::uint32_t packedColor =
        ((((static_cast<std::uint32_t>(color[0]) | 0xFFFF8000u) << 8) |
          color[1]) << 8) | color[2];

    const float lowerX = centerX - extentX;
    const float lowerY = centerY - extentY;
    const float lowerZ = centerZ - extentZ;
    const float upperX = extentX + centerX;
    const float upperY = extentY + centerY;
    const float upperZ = extentZ + centerZ;

    alignas(8) std::uint8_t vertexBytes[0x150] = {};
    for (unsigned int corner = 0; corner < 8; ++corner)
    {
        std::uint8_t* vertex = vertexBytes + corner * 0x24;
        CSF_AT(float, vertex, 0x00) = (corner & 1) ? upperX : lowerX;
        CSF_AT(float, vertex, 0x04) = (corner & 2) ? upperY : lowerY;
        CSF_AT(float, vertex, 0x08) = (corner & 4) ? upperZ : lowerZ;
        CSF_AT(std::uint32_t, vertex, 0x18) = packedColor;
    }

    int result = RwIm3DTransform(reinterpret_cast<int>(vertexBytes), 8, 0, 0);
    if (result)
        result = RwIm3DEnd();
    return reinterpret_cast<float*>(result);
}

HOOK(0x006E3080, GfxIm3D_TransformLightWorldBounds, "auto");
