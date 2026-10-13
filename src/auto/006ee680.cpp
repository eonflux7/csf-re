#include <cstdint>
#include "globals.h"
#include <cstddef>
#include <cstring>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, S00402350_InitTransform, 0x00402350, (S_00402350* self, void*));
CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(float*, __fastcall, S_006abb80_InitializeBounds, 0x006ABB80, (S_006abb80* self, void* , float xFirst, float xSecond, float yFirst, float ySecond, float zFirst, float zSecond));
CSF_FUNC(float*, __fastcall, GfxMatrix_GetBoundingSphere, 0x006AC530, (float* self, void* , float* output));
CSF_FUNC(int, __fastcall, SceneLoader_ConfigureGeometry, 0x006C35C0, (void* self, void* , int entryCount, int secondaryCount));
CSF_FUNC(int, __fastcall, SceneLoader_AddColoredVertex, 0x006C3630, (void* self, void* , const uint32_t* position, const uint8_t* color));
CSF_FUNC(int, __fastcall, C_00860848_Construct, 0x006C4490, (C_00860848* self, void*));
CSF_FUNC(char, __fastcall, GfxPtankCreate_GrowOutlineEntries, 0x006EE5D0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_00867358_AddOutlineIndex, 0x007387E0, (C_00867358* self, void* , int16_t value));

static int __cdecl GfxPtankCreate_AppendMeshOutline(int meshAddress, uint32_t* entries)
{
    void* mesh = reinterpret_cast<void*>(meshAddress);

    alignas(8) uint8_t boundsBytes[0x3C];
    alignas(8) uint8_t sphereBytes[0x14];
    alignas(8) uint8_t colorBytes[4] = {255, 255, 255, 255};
    std::memset(sphereBytes, 0, 16);

    if (CSF_AT(uint16_t, mesh, 0x84) != 0 &&
        CSF_AT(uint32_t, mesh, 0x08) != 0)
    {
        // Packet address markers for the two stack-object initializers.
        (void)CSF_VTBL(0x00402350);
        (void)CSF_VTBL(0x006ABB80);

        S_00402350& bounds = *reinterpret_cast<S_00402350*>(boundsBytes);
        S00402350_InitTransform(&bounds, nullptr);
        S_006abb80_InitializeBounds(
            reinterpret_cast<S_006abb80*>(boundsBytes), nullptr,
            CSF_AT(float, mesh, 0x48),
            CSF_AT(float, mesh, 0x54),
            CSF_AT(float, mesh, 0x4C),
            CSF_AT(float, mesh, 0x58),
            CSF_AT(float, mesh, 0x50),
            CSF_AT(float, mesh, 0x5C));
        GfxMatrix_GetBoundingSphere(
            reinterpret_cast<float*>(boundsBytes), nullptr,
            reinterpret_cast<float*>(sphereBytes));

        int allocation = CoreMemory_AllocUntagged(52);
        C_00867358* outline = allocation
            ? reinterpret_cast<C_00867358*>(C_00860848_Construct(
                  reinterpret_cast<C_00860848*>(allocation), nullptr))
            : nullptr;
        CSF_AT(C_00867358*, sphereBytes, 0x10) = outline;

        SceneLoader_ConfigureGeometry(
            outline, nullptr,
            CSF_AT(uint16_t, mesh, 0x82),
            6 * CSF_AT(uint16_t, mesh, 0x84));
        outline->field_001c = 1;

        uint32_t vertexIndex = 0;
        uint32_t vertexOffset = 0;
        if (CSF_AT(uint16_t, mesh, 0x82) != 0)
        {
            do
            {
                SceneLoader_AddColoredVertex(
                    outline, nullptr,
                    reinterpret_cast<const uint32_t*>(
                        CSF_AT(uint32_t, mesh, 0x08) + vertexOffset),
                    colorBytes);
                ++vertexIndex;
                vertexOffset += 12;
            }
            while (vertexIndex < CSF_AT(uint16_t, mesh, 0x82));
        }

        for (uint32_t triangleIndex = 0;
             triangleIndex < CSF_AT(uint16_t, mesh, 0x84);
             ++triangleIndex)
        {
            C_00867358_AddOutlineIndex(
                outline, nullptr,
                CSF_AT(int16_t, reinterpret_cast<void*>(
                    CSF_AT(uint32_t, mesh, 0x04)), 8 * triangleIndex));
            C_00867358_AddOutlineIndex(
                outline, nullptr,
                CSF_AT(int16_t, reinterpret_cast<void*>(
                    CSF_AT(uint32_t, mesh, 0x04)), 8 * triangleIndex + 2));
            C_00867358_AddOutlineIndex(
                outline, nullptr,
                CSF_AT(int16_t, reinterpret_cast<void*>(
                    CSF_AT(uint32_t, mesh, 0x04)), 8 * triangleIndex + 2));
            C_00867358_AddOutlineIndex(
                outline, nullptr,
                CSF_AT(int16_t, reinterpret_cast<void*>(
                    CSF_AT(uint32_t, mesh, 0x04)), 8 * triangleIndex + 4));
            C_00867358_AddOutlineIndex(
                outline, nullptr,
                CSF_AT(int16_t, reinterpret_cast<void*>(
                    CSF_AT(uint32_t, mesh, 0x04)), 8 * triangleIndex + 4));
            C_00867358_AddOutlineIndex(
                outline, nullptr,
                CSF_AT(int16_t, reinterpret_cast<void*>(
                    CSF_AT(uint32_t, mesh, 0x04)), 8 * triangleIndex));
        }

        if (entries[2] >= entries[1])
            GfxPtankCreate_GrowOutlineEntries(
                entries, nullptr, static_cast<int>(entries[3]));

        uint32_t entryIndex = entries[2];
        void* destination = reinterpret_cast<void*>(entries[0] + 20 * entryIndex);
        entries[2] = entryIndex + 1;
        CSF_AT(uint32_t, destination, 0x00) = CSF_AT(uint32_t, sphereBytes, 0x00);
        CSF_AT(uint32_t, destination, 0x04) = CSF_AT(uint32_t, sphereBytes, 0x04);
        CSF_AT(uint32_t, destination, 0x08) = CSF_AT(uint32_t, sphereBytes, 0x08);
        CSF_AT(uint32_t, destination, 0x0C) = CSF_AT(uint32_t, sphereBytes, 0x0C);
        CSF_AT(C_00867358*, destination, 0x10) = outline;
    }

    return meshAddress;
}

HOOK(0x006EE680, GfxPtankCreate_AppendMeshOutline, "auto");
