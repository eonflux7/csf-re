#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, SceneLoader_CreateIndexedGeometry, 0x006B1DE0,
         (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, SceneLoader_AddColoredVertex, 0x006C3630,
         (void* self, void* edx, uint32_t* a2, uint8_t* a3));
CSF_FUNC(int, __fastcall, SceneLoader_AddOutlineIndex, 0x007387E0,
         (void* self, void* edx, int16_t a2));

static int __stdcall SceneLoader_CreateSectorOutline(int sectorAddress)
{
    auto* sector = reinterpret_cast<uint8_t*>(sectorAddress);
    int color = sectorAddress;
    switch (CSF_AT(int, sector, 4))
    {
    case 0:
        color = -65536;
        break;
    case 1:
        color = -16711936;
        break;
    case 2:
        color = -16776961;
        break;
    }

    void* manager = reinterpret_cast<void*>(GetGlobalInt());
    auto* geometry = static_cast<C_00867358*>(
        SceneLoader_CreateIndexedGeometry(manager, nullptr, 4, 8));
    geometry->field_001c = 1;

    SceneLoader_AddColoredVertex(
        geometry, nullptr, reinterpret_cast<uint32_t*>(sector + 8),
        reinterpret_cast<uint8_t*>(&color));
    SceneLoader_AddColoredVertex(
        geometry, nullptr, reinterpret_cast<uint32_t*>(sector + 20),
        reinterpret_cast<uint8_t*>(&color));
    SceneLoader_AddColoredVertex(
        geometry, nullptr, reinterpret_cast<uint32_t*>(sector + 32),
        reinterpret_cast<uint8_t*>(&color));
    SceneLoader_AddColoredVertex(
        geometry, nullptr, reinterpret_cast<uint32_t*>(sector + 44),
        reinterpret_cast<uint8_t*>(&color));

    SceneLoader_AddOutlineIndex(geometry, nullptr, 0);
    SceneLoader_AddOutlineIndex(geometry, nullptr, 1);
    SceneLoader_AddOutlineIndex(geometry, nullptr, 1);
    SceneLoader_AddOutlineIndex(geometry, nullptr, 2);
    SceneLoader_AddOutlineIndex(geometry, nullptr, 2);
    SceneLoader_AddOutlineIndex(geometry, nullptr, 3);
    SceneLoader_AddOutlineIndex(geometry, nullptr, 3);
    int result = SceneLoader_AddOutlineIndex(geometry, nullptr, 0);

    CSF_AT(C_00867358*, sector, 64) = geometry;
    return result;
}

HOOK(0x004FDCA0, SceneLoader_CreateSectorOutline, "auto");
