#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);

CSF_FUNC(int, __fastcall, AiCore_GetSceneEntryCount, 0x0052E420, (void* self, void* edx));
CSF_FUNC(double, __cdecl, RwV2dLength, 0x00622FA0, (float* a1));
CSF_FUNC(bool, __fastcall, GfxPtank_TestBoundsOverlap, 0x006AB740, (void* self, void* edx, float* a2));
CSF_FUNC(char, __fastcall, GfxParticleRender_TestBoundaryFace, 0x006D7450, (void* self, void* edx, int a2, float* a3));
CSF_FUNC(int, __fastcall, ParticleEmitters_GetTableEntry, 0x006E0810, (void* self, void* , int index));

static char __fastcall GfxPtank_TestSweptBoundary(
    void* self, void* /*edx*/, float* position, float* dimensions)
{
    alignas(8) uint8_t boundsBytes[0x28]{};
    alignas(8) uint8_t deltaBytes[0x8]{};
    alignas(8) uint8_t pointBytes[0xC]{};

    float* bounds = reinterpret_cast<float*>(boundsBytes);
    float* delta = reinterpret_cast<float*>(deltaBytes);
    float* point = reinterpret_cast<float*>(pointBytes);

    const float halfWidth = dimensions[0] * g_HalfDurationScale;
    const float halfHeight = dimensions[1] * g_HalfDurationScale;

    bounds[3] = position[0] - halfWidth;
    bounds[4] = position[1] - halfHeight;
    bounds[5] = position[2] - halfWidth;
    bounds[6] = position[0] + halfWidth;
    bounds[0] = bounds[6];
    bounds[7] = position[1] + halfHeight;
    bounds[1] = bounds[7];
    bounds[8] = position[2] + halfWidth;
    bounds[2] = bounds[8];

    if (!GfxPtank_TestBoundsOverlap(
            static_cast<uint8_t*>(self) + 0x10, nullptr, bounds))
        return 0;

    const uint32_t entryCount = static_cast<uint32_t>(
        AiCore_GetSceneEntryCount(CSF_AT(void*, self, 0x4), nullptr));
    CSF_AT(uint32_t, pointBytes, 0x8) = entryCount;

    char result = 0;
    uint32_t index = 0;
    if (entryCount != 0)
    {
        do
        {
            if (result)
                break;

            void* entry = reinterpret_cast<void*>(
                ParticleEmitters_GetTableEntry(
                    CSF_AT(void*, self, 0x4), nullptr,
                    static_cast<int>(index)));

            const float entryZ = CSF_AT(float, entry, 0x14);
            point[1] = position[2];
            point[0] = position[0];
            delta[1] = entryZ - point[1];
            delta[0] = CSF_AT(float, entry, 0x10) - point[0];

            const float distance = static_cast<float>(RwV2dLength(delta));
            const float* reach = dimensions;
            // Unordered comparisons also select distance in the original.
            if (!(dimensions[0] < distance))
                reach = &distance;

            const float scale = *reach / distance;
            bounds[8] = delta[1] * scale;
            point[0] = delta[0] * scale + point[0];
            point[1] = bounds[8] + point[1];

            result = GfxParticleRender_TestBoundaryFace(
                CSF_AT(void*, self, 0x4), nullptr,
                static_cast<int>(index), point);
            ++index;
        }
        while (index < CSF_AT(uint32_t, pointBytes, 0x8));
    }
    return result;
}

HOOK(0x006D7720, GfxPtank_TestSweptBoundary, "auto");
