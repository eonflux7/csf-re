#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, S00402350_InitTransform, 0x00402350, (S_00402350* self, void*));
CSF_FUNC(unsigned int, __cdecl, RtRandom, 0x005867D0, ());
CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(void**, __fastcall, GfxWorld_FindSectorColor, 0x0069A8A0, (void* self, void* edx, void** a2, uint32_t* a3));
CSF_FUNC(float*, __fastcall, S_006abb80_InitializeBounds, 0x006ABB80, (S_006abb80* self, void* , float xFirst, float xSecond, float yFirst, float ySecond, float zFirst, float zSecond));
CSF_FUNC(float*, __fastcall, GfxMatrix_GetBoundingSphere, 0x006AC530, (float* self, void* , float* output));
CSF_FUNC(char, __fastcall, GfxWorld_GrowDebugGeometryArray, 0x006B7020, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GfxObjectProps_GetObjectSectorData, 0x006B87C0, (int objectIndex));
CSF_FUNC(int, __fastcall, SceneLoader_ConfigureGeometry, 0x006C35C0, (void* self, void* , int entryCount, int secondaryCount));
CSF_FUNC(int, __fastcall, SceneLoader_AddColoredVertex, 0x006C3630, (void* self, void* , const uint32_t* position, const uint8_t* color));
CSF_FUNC(int, __fastcall, SetEntryVec3AndPackedRgba, 0x006C3740, (void* self, void* , int index, const uint32_t* triplet, const uint8_t* packed));
CSF_FUNC(int, __fastcall, C_00860848_Construct, 0x006C4490, (C_00860848* self, void*));
CSF_FUNC(int, __fastcall, C_00867358_AddOutlineIndex, 0x007387E0, (C_00867358* self, void* , int16_t value));

static int __cdecl GfxWorld_AddSectorDebugGeometry(int sectorAddress, int contextAddress)
{
    auto* sector = reinterpret_cast<uint8_t*>(sectorAddress);
    auto* context = reinterpret_cast<uint8_t*>(contextAddress);

    alignas(8) uint8_t boundsBytes[0x3C];
    alignas(8) uint8_t recordBytes[0x20];
    alignas(8) uint8_t lookupBytes[0x8];
    alignas(8) uint8_t keyBytes[0x4];
    alignas(8) uint8_t vertexColors[0x8];
    alignas(8) uint8_t sectorColor[0x8] = {};

    // These packet-listed identities denote the three directly called routines,
    // not vtable stores in this function.
    (void)CSF_VTBL(0x00402350);
    (void)CSF_VTBL(0x006ABB80);
    (void)CSF_VTBL(0x0069A8A0);

    CSF_AT(uint8_t*, lookupBytes, 0x4) = context + 0x14;
    sectorColor[7] = CSF_AT(uint8_t, context, 0x25);

    vertexColors[4] = static_cast<uint8_t>(RtRandom());
    vertexColors[5] = static_cast<uint8_t>(RtRandom());
    vertexColors[6] = static_cast<uint8_t>(RtRandom());
    const bool hasTriangles = CSF_AT(uint16_t, sector, 0x84) != 0;
    vertexColors[7] = 0xFF;
    vertexColors[0] = 0;
    vertexColors[1] = 0;
    vertexColors[2] = 0;
    vertexColors[3] = 0xFF;
    std::memset(recordBytes, 0, 24);
    CSF_AT(uint32_t, recordBytes, 0x18) =
        CSF_AT(uint32_t, vertexColors, 0x4);

    if (!hasTriangles)
        return sectorAddress;

    S00402350_InitTransform(reinterpret_cast<S_00402350*>(boundsBytes), nullptr);
    S_006abb80_InitializeBounds(
        reinterpret_cast<S_006abb80*>(boundsBytes), nullptr,
        CSF_AT(float, sector, 0x48), CSF_AT(float, sector, 0x54),
        CSF_AT(float, sector, 0x4C), CSF_AT(float, sector, 0x58),
        CSF_AT(float, sector, 0x50), CSF_AT(float, sector, 0x5C));
    GfxMatrix_GetBoundingSphere(
        reinterpret_cast<float*>(boundsBytes), nullptr,
        reinterpret_cast<float*>(recordBytes));

    int allocation = CoreMemory_AllocUntagged(52);
    CSF_AT(C_00867358*, recordBytes, 0x14) = allocation
        ? reinterpret_cast<C_00867358*>(C_00860848_Construct(
              reinterpret_cast<C_00860848*>(allocation), nullptr))
        : nullptr;
    SceneLoader_ConfigureGeometry(
        CSF_AT(C_00867358*, recordBytes, 0x14), nullptr,
        CSF_AT(uint16_t, sector, 0x82),
        6 * CSF_AT(uint16_t, sector, 0x84));
    CSF_AT(C_00867358*, recordBytes, 0x14)->field_001c = 1;

    allocation = CoreMemory_AllocUntagged(52);
    CSF_AT(C_00867358*, recordBytes, 0x10) = allocation
        ? reinterpret_cast<C_00867358*>(C_00860848_Construct(
              reinterpret_cast<C_00860848*>(allocation), nullptr))
        : nullptr;
    SceneLoader_ConfigureGeometry(
        CSF_AT(C_00867358*, recordBytes, 0x10), nullptr,
        CSF_AT(uint16_t, sector, 0x82),
        3 * CSF_AT(uint16_t, sector, 0x84));
    CSF_AT(C_00867358*, recordBytes, 0x10)->field_001c = 3;

    uint32_t vertexIndex = 0;
    uint32_t vertexOffset = 0;
    if (CSF_AT(uint16_t, sector, 0x82) != 0)
    {
        do
        {
            SceneLoader_AddColoredVertex(
                CSF_AT(C_00867358*, recordBytes, 0x14), nullptr,
                reinterpret_cast<const uint32_t*>(
                    CSF_AT(uint8_t*, sector, 0x8) + vertexOffset),
                vertexColors);
            const uint8_t* color = sectorColor[7]
                ? vertexColors + 4 : sectorColor;
            SceneLoader_AddColoredVertex(
                CSF_AT(C_00867358*, recordBytes, 0x10), nullptr,
                reinterpret_cast<const uint32_t*>(
                    CSF_AT(uint8_t*, sector, 0x8) + vertexOffset),
                color);
            ++vertexIndex;
            vertexOffset += 12;
        }
        while (vertexIndex < CSF_AT(uint16_t, sector, 0x82));
    }

    uint32_t triangleIndex = 0;
    if (CSF_AT(uint16_t, sector, 0x84) != 0)
    {
        do
        {
            const uint32_t triangleOffset = 8 * triangleIndex;
            auto* triangle = CSF_AT(uint8_t*, sector, 0x4) + triangleOffset;
            auto* objectTable = CSF_AT(uint8_t*, context, 0x10);
            CSF_AT(uint32_t, sectorColor, 0) = 0xFFFFFFFF;
            const int objectIndex = CSF_AT(
                int, CSF_AT(uint8_t*, objectTable, 0x10),
                4 * CSF_AT(uint8_t, triangle, 0x6));
            if (objectIndex)
            {
                const int objectData = GfxObjectProps_GetObjectSectorData(objectIndex);
                if (objectData)
                {
                    CSF_AT(uint32_t, keyBytes, 0) =
                        CSF_AT(uint32_t, reinterpret_cast<void*>(objectData), 0x8);
                    GfxWorld_FindSectorColor(
                        context + 0x4, nullptr,
                        reinterpret_cast<void**>(lookupBytes),
                        reinterpret_cast<uint32_t*>(keyBytes));
                    if (CSF_AT(void*, lookupBytes, 0) !=
                        CSF_AT(void*, context, 0x8))
                    {
                        CSF_AT(uint32_t, sectorColor, 0) =
                            CSF_AT(uint32_t, CSF_AT(void*, lookupBytes, 0), 0x10);
                    }
                }
            }

            CSF_AT(uint32_t, keyBytes, 0) = CSF_AT(uint16_t, triangle, 0x2);
            CSF_AT(uint32_t, recordBytes, 0x1C) = CSF_AT(uint16_t, triangle, 0);
            const int thirdIndex = CSF_AT(uint16_t, triangle, 0x4);
            if (!sectorColor[7])
            {
                SetEntryVec3AndPackedRgba(
                    CSF_AT(C_00867358*, recordBytes, 0x10), nullptr,
                    CSF_AT(uint32_t, recordBytes, 0x1C), nullptr, sectorColor);
                SetEntryVec3AndPackedRgba(
                    CSF_AT(C_00867358*, recordBytes, 0x10), nullptr,
                    CSF_AT(uint32_t, keyBytes, 0), nullptr, sectorColor);
                SetEntryVec3AndPackedRgba(
                    CSF_AT(C_00867358*, recordBytes, 0x10), nullptr,
                    thirdIndex, nullptr, sectorColor);
            }

            C_00867358_AddOutlineIndex(
                CSF_AT(C_00867358*, recordBytes, 0x10), nullptr,
                static_cast<int16_t>(CSF_AT(uint32_t, recordBytes, 0x1C)));
            C_00867358_AddOutlineIndex(
                CSF_AT(C_00867358*, recordBytes, 0x10), nullptr,
                CSF_AT(int16_t, keyBytes, 0));
            C_00867358_AddOutlineIndex(
                CSF_AT(C_00867358*, recordBytes, 0x10), nullptr,
                static_cast<int16_t>(thirdIndex));

            C_00867358_AddOutlineIndex(
                CSF_AT(C_00867358*, recordBytes, 0x14), nullptr,
                CSF_AT(int16_t, CSF_AT(uint8_t*, sector, 0x4), 8 * triangleIndex));
            C_00867358_AddOutlineIndex(
                CSF_AT(C_00867358*, recordBytes, 0x14), nullptr,
                CSF_AT(int16_t, CSF_AT(uint8_t*, sector, 0x4), triangleOffset + 2));
            C_00867358_AddOutlineIndex(
                CSF_AT(C_00867358*, recordBytes, 0x14), nullptr,
                CSF_AT(int16_t, CSF_AT(uint8_t*, sector, 0x4), triangleOffset + 2));
            C_00867358_AddOutlineIndex(
                CSF_AT(C_00867358*, recordBytes, 0x14), nullptr,
                CSF_AT(int16_t, CSF_AT(uint8_t*, sector, 0x4), triangleOffset + 4));
            C_00867358_AddOutlineIndex(
                CSF_AT(C_00867358*, recordBytes, 0x14), nullptr,
                CSF_AT(int16_t, CSF_AT(uint8_t*, sector, 0x4), triangleOffset + 4));
            C_00867358_AddOutlineIndex(
                CSF_AT(C_00867358*, recordBytes, 0x14), nullptr,
                CSF_AT(int16_t, CSF_AT(uint8_t*, sector, 0x4), 8 * triangleIndex));
            ++triangleIndex;
        }
        while (triangleIndex < CSF_AT(uint16_t, sector, 0x84));
    }

    auto* records = CSF_AT(uint8_t*, lookupBytes, 0x4);
    if (CSF_AT(uint32_t, records, 0x8) >= CSF_AT(uint32_t, records, 0x4))
        GfxWorld_GrowDebugGeometryArray(
            records, nullptr, CSF_AT(int, records, 0xC));

    auto* destination = CSF_AT(uint8_t*, records, 0) +
        28 * CSF_AT(uint32_t, records, 0x8)++;
    std::memcpy(destination, recordBytes, 0x1C);
    return sectorAddress;
}

HOOK(0x006B7310, GfxWorld_AddSectorDebugGeometry, "auto");
