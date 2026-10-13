#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int*, __fastcall, GfxWorld_UpdateTrianglePackedColor, 0x006B7120,
         (void* self, void* edx, int* a2, uint8_t* a3));
CSF_FUNC(int, __fastcall, SetEntryVec3AndPackedRgba, 0x006C3740,
         (void* self, void* , int index, const uint32_t* triplet, const uint8_t* packed));

static int __cdecl GfxWorld_ApplySectorTriangleColors(int sectorAddress, int* context)
{
    void* sector = reinterpret_cast<void*>(
        static_cast<uintptr_t>(static_cast<uint32_t>(sectorAddress)));

    if (CSF_AT(uint16_t, sector, 0x84) == 0)
        return sectorAddress;

    void* world = CSF_AT(void*, context, 0);
    const uint8_t colorMode = CSF_AT(uint8_t, world, 0x25);
    const uint32_t materialIndex = static_cast<uint32_t>(context[1]);
    uint8_t* material =
        CSF_AT(uint8_t*, world, 0x14) + 28 * materialIndex;

    alignas(8) uint8_t packedBytes[0x8] = {};
    packedBytes[7] = colorMode;
    context[1] = static_cast<int>(materialIndex + 1);
    CSF_AT(uint32_t, packedBytes, 0) = CSF_AT(uint32_t, material, 0x18);

    uint32_t triangleIndex = 0;
    if (CSF_AT(uint16_t, sector, 0x84) != 0)
    {
        do
        {
            int* triangle = reinterpret_cast<int*>(
                CSF_AT(uint8_t*, sector, 0x4) + 8 * triangleIndex);

            if (packedBytes[7] == 0)
            {
                GfxWorld_UpdateTrianglePackedColor(
                    CSF_AT(void*, context, 0), nullptr, triangle, packedBytes);
            }

            const int secondVertex = CSF_AT(uint16_t, triangle, 0x2);
            const int thirdVertex = CSF_AT(uint16_t, triangle, 0x4);
            const int firstVertex = CSF_AT(uint16_t, triangle, 0);

            SetEntryVec3AndPackedRgba(
                CSF_AT(void*, material, 0x10), nullptr,
                firstVertex, nullptr, packedBytes);
            SetEntryVec3AndPackedRgba(
                CSF_AT(void*, material, 0x10), nullptr,
                secondVertex, nullptr, packedBytes);
            SetEntryVec3AndPackedRgba(
                CSF_AT(void*, material, 0x10), nullptr,
                thirdVertex, nullptr, packedBytes);

            ++triangleIndex;
        } while (triangleIndex < CSF_AT(uint16_t, sector, 0x84));
    }

    return sectorAddress;
}

HOOK(0x006B7240, GfxWorld_ApplySectorTriangleColors, "auto");
