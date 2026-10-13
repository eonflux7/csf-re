#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, RwIm3DTransform, 0x0063CED0, (int a1, unsigned int a2, int a3, int a4));
CSF_FUNC(int, __cdecl, RwIm3DEnd, 0x0063CFA0, ());
CSF_FUNC(int, __cdecl, RwIm3DRenderIndexedPrimitive, 0x0063CFD0, (int a1, int a2, int a3));

CSF_GLOBAL(float, g_AzimuthStep, 0x00861A4C);
CSF_GLOBAL(float, g_RingStart, 0x008BB934);
CSF_GLOBAL(float, g_RingEnd, 0x008BBA9C);
CSF_GLOBAL(std::uint32_t, g_CenterX, 0x008BB910);
CSF_GLOBAL(std::uint32_t, g_CenterY, 0x008BB914);
CSF_GLOBAL(std::uint32_t, g_CenterZ, 0x008BB918);
CSF_GLOBAL(std::uint32_t, g_CenterColor, 0x008BB928);

CSF_GLOBAL(std::uint16_t, g_Index00, 0x008BB898);
CSF_GLOBAL(std::uint16_t, g_Index01, 0x008BB89A);
CSF_GLOBAL(std::uint16_t, g_Index02, 0x008BB89C);
CSF_GLOBAL(std::uint16_t, g_Index03, 0x008BB89E);
CSF_GLOBAL(std::uint16_t, g_Index04, 0x008BB8A0);
CSF_GLOBAL(std::uint16_t, g_Index05, 0x008BB8A2);
CSF_GLOBAL(std::uint16_t, g_Index06, 0x008BB8A4);
CSF_GLOBAL(std::uint16_t, g_Index07, 0x008BB8A6);
CSF_GLOBAL(std::uint16_t, g_Index08, 0x008BB8A8);
CSF_GLOBAL(std::uint16_t, g_Index09, 0x008BB8AA);
CSF_GLOBAL(std::uint16_t, g_Index10, 0x008BB8AC);
CSF_GLOBAL(std::uint16_t, g_Index11, 0x008BB8AE);
CSF_GLOBAL(std::uint16_t, g_Index12, 0x008BB8B0);
CSF_GLOBAL(std::uint16_t, g_Index13, 0x008BB8B2);
CSF_GLOBAL(std::uint16_t, g_Index14, 0x008BB8B4);
CSF_GLOBAL(std::uint16_t, g_Index15, 0x008BB8B6);
CSF_GLOBAL(std::uint16_t, g_Index16, 0x008BB8B8);
CSF_GLOBAL(std::uint16_t, g_Index17, 0x008BB8BA);
CSF_GLOBAL(std::uint16_t, g_Index18, 0x008BB8BC);
CSF_GLOBAL(std::uint16_t, g_Index19, 0x008BB8BE);
CSF_GLOBAL(std::uint16_t, g_Index20, 0x008BB8C0);
CSF_GLOBAL(std::uint16_t, g_Index21, 0x008BB8C2);
CSF_GLOBAL(std::uint16_t, g_Index22, 0x008BB8C4);
CSF_GLOBAL(std::uint16_t, g_Index23, 0x008BB8C6);
CSF_GLOBAL(std::uint16_t, g_Index24, 0x008BB8C8);
CSF_GLOBAL(std::uint16_t, g_Index25, 0x008BB8CA);
CSF_GLOBAL(std::uint16_t, g_Index26, 0x008BB8CC);
CSF_GLOBAL(std::uint16_t, g_Index27, 0x008BB8CE);
CSF_GLOBAL(std::uint16_t, g_Index28, 0x008BB8D0);
CSF_GLOBAL(std::uint16_t, g_Index29, 0x008BB8D2);
CSF_GLOBAL(std::uint16_t, g_Index30, 0x008BB8D4);
CSF_GLOBAL(std::uint16_t, g_Index31, 0x008BB8D6);
CSF_GLOBAL(std::uint16_t, g_Index32, 0x008BB8D8);
CSF_GLOBAL(std::uint16_t, g_Index33, 0x008BB8DA);
CSF_GLOBAL(std::uint16_t, g_Index34, 0x008BB8DC);
CSF_GLOBAL(std::uint16_t, g_Index35, 0x008BB8DE);
CSF_GLOBAL(std::uint16_t, g_Index36, 0x008BB8E0);
CSF_GLOBAL(std::uint16_t, g_Index37, 0x008BB8E2);
CSF_GLOBAL(std::uint16_t, g_Index38, 0x008BB8E4);
CSF_GLOBAL(std::uint16_t, g_Index39, 0x008BB8E6);
CSF_GLOBAL(std::uint16_t, g_Index40, 0x008BB8E8);
CSF_GLOBAL(std::uint16_t, g_Index41, 0x008BB8EA);
CSF_GLOBAL(std::uint16_t, g_Index42, 0x008BB8EC);
CSF_GLOBAL(std::uint16_t, g_Index43, 0x008BB8EE);
CSF_GLOBAL(std::uint16_t, g_Index44, 0x008BB8F0);
CSF_GLOBAL(std::uint16_t, g_Index45, 0x008BB8F2);
CSF_GLOBAL(std::uint16_t, g_Index46, 0x008BB8F4);
CSF_GLOBAL(std::uint16_t, g_Index47, 0x008BB8F6);
CSF_GLOBAL(std::uint16_t, g_Index48, 0x008BB8F8);
CSF_GLOBAL(std::uint16_t, g_Index49, 0x008BB8FA);
CSF_GLOBAL(std::uint16_t, g_Index50, 0x008BB8FC);
CSF_GLOBAL(std::uint16_t, g_Index51, 0x008BB8FE);
CSF_GLOBAL(std::uint16_t, g_Index52, 0x008BB900);
CSF_GLOBAL(std::uint16_t, g_Index53, 0x008BB902);
CSF_GLOBAL(std::uint16_t, g_Index54, 0x008BB904);
CSF_GLOBAL(std::uint16_t, g_Index55, 0x008BB906);
CSF_GLOBAL(std::uint16_t, g_Index56, 0x008BB908);
CSF_GLOBAL(std::uint16_t, g_Index57, 0x008BB90A);
CSF_GLOBAL(std::uint16_t, g_Index58, 0x008BB90C);
CSF_GLOBAL(std::uint16_t, g_Index59, 0x008BB90E);

static int __stdcall GfxIm3D_DrawTransformedCircularFan(
    float angle, float radius, unsigned char* color, float* matrix)
{
    int azimuthIndex = 0;
    auto* vertex = reinterpret_cast<std::uint8_t*>(&g_RingStart);
    const float axialDistance = std::cos(angle) * radius;
    const float angleSine = std::sin(angle);

    do
    {
        const float azimuth = static_cast<float>(azimuthIndex) * g_AzimuthStep;
        const std::uint32_t rimColor =
            color[2] |
            ((static_cast<std::uint32_t>(color[1]) |
              ((static_cast<std::uint32_t>(color[0]) | 0xFFFF8000u) << 8)) << 8);

        const float localY = std::sin(azimuth) * angleSine * radius;
        const float yContributionX = localY * matrix[4];
        const float yContributionY = localY * matrix[5];
        const float yContributionZ = localY * matrix[6];

        const float localX = std::cos(azimuth) * angleSine * radius;
        const float xContributionX = localX * matrix[0];
        const float xContributionY = localX * matrix[1];
        const float xContributionZ = localX * matrix[2];
        const float axialX = axialDistance * matrix[8];
        const float axialY = axialDistance * matrix[9];
        const float axialZ = axialDistance * matrix[10];

        const float worldX =
            axialX + matrix[12] + xContributionX + yContributionX;
        const float worldY =
            axialY + matrix[13] + xContributionY + yContributionY;
        const float worldZ =
            axialZ + xContributionZ + yContributionZ + matrix[14];

        CSF_AT(std::uint32_t, vertex, 0x18) = rimColor;
        CSF_AT(float, vertex, 0x00) = worldX;
        CSF_AT(float, vertex, 0x08) = worldZ;
        CSF_AT(float, vertex, 0x04) = worldY;

        azimuthIndex += 2;
        vertex += 0x24;
    }
    while (reinterpret_cast<std::intptr_t>(vertex) <
           reinterpret_cast<std::intptr_t>(&g_RingEnd));

    g_CenterColor = (static_cast<std::uint32_t>(color[2]) + 128) |
                   ((static_cast<std::uint32_t>(color[0]) + 128) << 16) |
                   ((static_cast<std::uint32_t>(color[1]) + 128) << 8) |
                   0x80000000u;
    g_CenterX = CSF_AT(std::uint32_t, matrix, 0x30);
    g_CenterY = CSF_AT(std::uint32_t, matrix, 0x34);
    const std::uint32_t centerZ = CSF_AT(std::uint32_t, matrix, 0x38);

    g_Index01 = 2;
    g_Index05 = 2;
    g_Index32 = 2;
    g_Index34 = 2;
    g_CenterZ = centerZ;
    g_Index35 = 3;
    g_Index37 = 3;
    g_Index04 = 3;
    g_Index07 = 4;
    g_Index08 = 3;
    g_Index11 = 4;
    g_Index38 = 4;
    g_Index40 = 4;
    g_Index10 = 5;
    g_Index13 = 6;
    g_Index14 = 5;
    g_Index17 = 6;
    g_Index41 = 5;
    g_Index43 = 5;
    g_Index00 = 0;
    g_Index02 = 1;
    g_Index03 = 0;
    g_Index06 = 0;
    g_Index09 = 0;
    g_Index12 = 0;
    g_Index15 = 0;
    g_Index16 = 7;
    g_Index18 = 0;
    g_Index19 = 8;
    g_Index20 = 7;
    g_Index21 = 0;
    g_Index22 = 9;
    g_Index23 = 8;
    g_Index24 = 0;
    g_Index25 = 10;
    g_Index26 = 9;
    g_Index27 = 0;
    g_Index29 = 10;
    g_Index28 = 1;
    g_Index30 = 0;
    g_Index31 = 1;
    g_Index33 = 0;
    g_Index36 = 0;
    g_Index39 = 0;
    g_Index42 = 0;
    g_Index44 = 6;
    g_Index45 = 0;
    g_Index46 = 6;
    g_Index47 = 7;
    g_Index48 = 0;
    g_Index49 = 7;
    g_Index50 = 8;
    g_Index51 = 0;
    g_Index52 = 8;
    g_Index53 = 9;
    g_Index54 = 0;
    g_Index55 = 9;
    g_Index56 = 10;
    g_Index57 = 0;
    g_Index58 = 10;
    g_Index59 = 1;

    const int result = RwIm3DTransform(
        static_cast<int>(reinterpret_cast<std::intptr_t>(&g_CenterX)), 11, 0, 0);
    if (result)
    {
        RwIm3DRenderIndexedPrimitive(
            3, static_cast<int>(reinterpret_cast<std::intptr_t>(&g_Index00)), 60);
        return RwIm3DEnd();
    }
    return result;
}

HOOK(0x006E33E0, GfxIm3D_DrawTransformedCircularFan, "auto");
