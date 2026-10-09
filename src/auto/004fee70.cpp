#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, SceneLoader_MarkSectorUsed, 0x004E8E00,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, SceneLoader_ResolveSectorIndex, 0x004FEE10,
         (uint32_t* a1, uint8_t* a2));
CSF_FUNC(int, __cdecl, SceneLoader_GetObjectSectorData, 0x006B87C0,
         (int a1));
CSF_FUNC(void*, __cdecl, SceneLoader_PrepareObject, 0x006BB8A0,
         (uint32_t* a1));
CSF_FUNC(char, __cdecl, SceneLoader_SetObjectPlacement, 0x006C4EB0,
         (int* a1, int a2));

CSF_GLOBAL(uint32_t, g_UnassignedSectorFlags, 0x00869C54);

static int __cdecl SceneLoader_AssignObjectSector(int object, int placement)
{
    (void)CSF_VTBL(0x004E8E00);

    SceneLoader_PrepareObject(reinterpret_cast<uint32_t*>(object));
    void* sectorData =
        reinterpret_cast<void*>(SceneLoader_GetObjectSectorData(object));
    void* placementData = reinterpret_cast<void*>(placement);

    if (sectorData)
    {
        uint8_t sectorByte = static_cast<uint8_t>(
            (static_cast<uint32_t>(placement) >> 24) & 0xFFu);
        int sectorIndex = SceneLoader_ResolveSectorIndex(
            CSF_AT(uint32_t*, sectorData, 0x4), &sectorByte);

        CSF_AT(uint8_t, sectorData, 0x10) = sectorByte;

        if (sectorIndex == -1)
        {
            CSF_AT(uint32_t, sectorData, 0xC) |= g_UnassignedSectorFlags;
            CSF_AT(int, sectorData, 0x8) = -1;
        }
        else
        {
            void* scene = reinterpret_cast<void*>(GetValue8B0554());
            void* sectors = CSF_AT(void*, scene, 0x1FD4);
            void* sector =
                ((void* (__fastcall*)(void*, void*, int))
                     CSF_AT(void**, sectors, 0)[8 / 4])(
                    sectors, nullptr, sectorIndex);

            CSF_AT(int, sectorData, 0x8) = sectorIndex;
            CSF_AT(uint32_t, sectorData, 0xC) |=
                CSF_AT(uint32_t, sector, 0x20);
            CSF_AT(uint8_t, placementData, 0x1) = 1;

            scene = reinterpret_cast<void*>(GetValue8B0554());
            SceneLoader_MarkSectorUsed(scene, nullptr, sectorIndex);
        }
    }

    int placementFirst = CSF_AT(int, placementData, 0x4);
    int placementSecond = CSF_AT(int, placementData, 0x8);
    alignas(4) uint8_t placementDescriptor[0xC] = {};
    placementDescriptor[0] = 0;
    CSF_AT(int, placementDescriptor, 0x4) = placementFirst;
    CSF_AT(int, placementDescriptor, 0x8) = placementSecond;

    SceneLoader_SetObjectPlacement(
        reinterpret_cast<int*>(object),
        reinterpret_cast<int>(placementDescriptor));
    return object;
}

HOOK(0x004FEE70, SceneLoader_AssignObjectSector, "auto");
