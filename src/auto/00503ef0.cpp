#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0, (void* self, void* edx, int growth));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(void, __fastcall, CSceneLoader_CreatePlacedObject, 0x004FF500, (void* self, void* edx, int a2, float* a3, float* a4, float a5, int a6, int a7));
CSF_FUNC(int, __stdcall, SceneLoader_PrepareWorld, 0x00501360, (int a1));
CSF_FUNC(unsigned int, __fastcall, CSceneLoader_ResolveObjectName, 0x00503BF0, (void* self, void* edx, char* source));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, RpWorldForAllClumps, 0x0058CEF0, (int world, int (__cdecl* callback)(uint32_t*, int), int context));
CSF_FUNC(int, __cdecl, RpWorldForAllMaterials, 0x0058CF30, (int world, int (__cdecl* callback)(uint32_t, int), int context));
CSF_FUNC(int, __fastcall, SceneLoader_GetWorldManager, 0x00652010, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetRenderWorld, 0x006B6AF0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_ReleaseWorldAtIndex, 0x006CBA40, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, SceneLoader_LoadWorld, 0x006CDAA0, (void* self, void* edx, char* a2, void* a3, int a4));

static const auto g_ArrayVtableReference = CSF_VTBL(0x0041E5F0);

static int __fastcall CSceneLoader_LoadSectorWorlds(void* self, void* /*edx*/)
{
    uint32_t loadedWorlds[4] = {0, 0, 0, 10};
    uint32_t loadedCount = 0;

    void* manager = reinterpret_cast<void*>(
        SceneLoader_GetWorldManager(reinterpret_cast<void*>(GetGlobalInt()), nullptr));
    auto* sectors = reinterpret_cast<uint8_t*>(GetValue8B0554()) + 0x24;

    uint32_t sectorCount = CSF_AT(uint32_t, sectors, 0x04);
    if (sectorCount)
    {
        sectorCount = static_cast<uint32_t>(
            static_cast<int32_t>(CSF_AT(uint32_t, sectors, 0x08) - sectorCount) / 72);
        if (sectorCount)
        {
            GrowButtonArray(loadedWorlds, nullptr, static_cast<int>(sectorCount));
            loadedCount = loadedWorlds[2];
        }
    }

    uint32_t sectorOffset = 0;
    for (uint32_t remaining = sectorCount; remaining; --remaining, sectorOffset += 72)
    {
        auto* sector = reinterpret_cast<uint8_t*>(
            CSF_AT(uint32_t, sectors, 0x04)) + sectorOffset;

        int loadFlag = CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalState_50D190()), 0xD0);

        void* secondName;
        if (CSF_AT(uint32_t, sector, 0x34) < 0x10)
            secondName = sector + 0x20;
        else
            secondName = CSF_AT(void*, sector, 0x20);

        char* firstName;
        if (CSF_AT(uint32_t, sector, 0x18) < 0x10)
            firstName = reinterpret_cast<char*>(sector + 0x04);
        else
            firstName = CSF_AT(char*, sector, 0x04);

        int worldId = SceneLoader_LoadWorld(manager, nullptr, firstName, secondName, loadFlag);
        if (loadedCount >= loadedWorlds[1])
        {
            GrowButtonArray(loadedWorlds, nullptr, static_cast<int>(loadedWorlds[3]));
            loadedCount = loadedWorlds[2];
        }
        reinterpret_cast<int*>(loadedWorlds[0])[loadedCount] = worldId;
        loadedWorlds[2] = loadedCount + 1;

        int worldHandle =
            ((int (__fastcall*)(void*, void*, int))CSF_AT(void**, manager, 0)[0x90 / 4])(
                manager, nullptr, worldId);
        SceneLoader_PrepareWorld(worldHandle);

        void* renderObject =
            ((void* (__fastcall*)(void*, void*, int))CSF_AT(void**, manager, 0)[0x90 / 4])(
                manager, nullptr, worldId);
        int renderWorld = SceneLoader_GetRenderWorld(renderObject, nullptr);

        alignas(4) uint8_t renderContext[0x2C] = {};
        renderContext[0] = 0;
        void* settings = LookupObjectById(reinterpret_cast<void*>(GetGlobalInt()), nullptr);
        void* settingsData =
            ((void* (__fastcall*)(void*, void*))CSF_AT(void**, settings, 0)[0x18 / 4])(
                settings, nullptr);
        CSF_AT(uint32_t, renderContext, 0x04) = CSF_AT(uint32_t, settingsData, 0x20);

        settings = LookupObjectById(reinterpret_cast<void*>(GetGlobalInt()), nullptr);
        settingsData =
            ((void* (__fastcall*)(void*, void*))CSF_AT(void**, settings, 0)[0x18 / 4])(
                settings, nullptr);
        CSF_AT(uint32_t, renderContext, 0x08) = CSF_AT(uint32_t, settingsData, 0x1C);

        RpWorldForAllMaterials(
            renderWorld,
            reinterpret_cast<int (__cdecl*)(uint32_t, int)>(CSF_ADDR_FN(0x004FEE70)),
            reinterpret_cast<int>(renderContext));
        RpWorldForAllClumps(
            renderWorld,
            reinterpret_cast<int (__cdecl*)(uint32_t*, int)>(CSF_ADDR_FN(0x004FF000)),
            reinterpret_cast<int>(renderContext));

        void* reference =
            ((void* (__fastcall*)(void*, void*, int))CSF_AT(void**, manager, 0)[0x8C / 4])(
                manager, nullptr, worldId);
        int referencedWorld = reinterpret_cast<int>(GetReferencedObject(reference, nullptr));

        alignas(4) uint8_t referencedContext[0x0C] = {};
        referencedContext[0] = 1;
        settings = LookupObjectById(reinterpret_cast<void*>(GetGlobalInt()), nullptr);
        settingsData =
            ((void* (__fastcall*)(void*, void*))CSF_AT(void**, settings, 0)[0x18 / 4])(
                settings, nullptr);
        CSF_AT(uint32_t, referencedContext, 0x04) = CSF_AT(uint32_t, settingsData, 0x20);

        settings = LookupObjectById(reinterpret_cast<void*>(GetGlobalInt()), nullptr);
        settingsData =
            ((void* (__fastcall*)(void*, void*))CSF_AT(void**, settings, 0)[0x18 / 4])(
                settings, nullptr);
        CSF_AT(uint32_t, referencedContext, 0x08) = CSF_AT(uint32_t, settingsData, 0x1C);

        RpWorldForAllMaterials(
            referencedWorld,
            reinterpret_cast<int (__cdecl*)(uint32_t, int)>(CSF_ADDR_FN(0x004FEE70)),
            reinterpret_cast<int>(referencedContext));
        RpWorldForAllClumps(
            referencedWorld,
            reinterpret_cast<int (__cdecl*)(uint32_t*, int)>(CSF_ADDR_FN(0x004FF000)),
            reinterpret_cast<int>(referencedContext));

        uint32_t objectIndex = 0;
        uint32_t objectOffset = 0;
        for (;;)
        {
            uint32_t objectsBegin = CSF_AT(uint32_t, sector, 0x3C);
            if (!objectsBegin)
                break;
            uint32_t objectCount = static_cast<uint32_t>(
                static_cast<int32_t>(CSF_AT(uint32_t, sector, 0x40) - objectsBegin) / 60);
            if (objectIndex >= objectCount)
                break;

            auto* currentSector = reinterpret_cast<uint8_t*>(
                CSF_AT(uint32_t, sectors, 0x04)) + sectorOffset;
            auto* object = reinterpret_cast<uint8_t*>(
                CSF_AT(uint32_t, currentSector, 0x3C)) + objectOffset;

            char* name;
            if (CSF_AT(uint32_t, object, 0x38) < 0x10)
                name = reinterpret_cast<char*>(object + 0x24);
            else
                name = CSF_AT(char*, object, 0x24);

            unsigned int objectId = CSceneLoader_ResolveObjectName(self, nullptr, name);
            CSceneLoader_CreatePlacedObject(
                self, nullptr, CSF_AT(int, object, 0),
                reinterpret_cast<float*>(object + 0x04),
                reinterpret_cast<float*>(object + 0x10),
                CSF_AT(float, object, 0x1C), static_cast<int>(objectId), worldId);

            ++objectIndex;
            objectOffset += 60;
        }
        loadedCount = loadedWorlds[2];
    }

    for (uint32_t index = 0;
         index < static_cast<uint32_t>(
             ((int (__fastcall*)(void*, void*))CSF_AT(void**, manager, 0)[0x88 / 4])(
                 manager, nullptr));
         ++index)
    {
        uint32_t match = 0;
        while (match < loadedCount &&
               reinterpret_cast<uint32_t*>(loadedWorlds[0])[match] != index)
            ++match;
        if (match < loadedCount && match != UINT32_MAX)
            continue;
        SceneLoader_ReleaseWorldAtIndex(manager, nullptr, static_cast<int>(index));
    }

    if (loadedWorlds[0])
        return Mem_Free(reinterpret_cast<void*>(loadedWorlds[0]));
    return 0;
}

HOOK(0x00503EF0, CSceneLoader_LoadSectorWorlds, "auto");
