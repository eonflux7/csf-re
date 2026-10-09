#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, GetSceneRegionAt, 0x004FC130, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, SceneLoader_GetResourceManager, 0x00652010, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, SceneLoader_ResolveRegionIndex, 0x00774C30, (void* self, void* edx, int a2));
CSF_FUNC(unsigned int, __fastcall, SceneLoader_AssignObjectResource, 0x00490D20,
         (C_00833908* self, void* edx, uint32_t resource, char mode));

static int __fastcall CSceneLoader_InitializeRegionResources(
    CSceneLoader* self, void* /*edx*/, int sceneIndex)
{
    const uint32_t objectCount = CSF_AT(uint32_t, g_pCurrentScene, 0x18);

    for (uint32_t index = 0; index < objectCount; ++index)
    {
        auto* object = CSF_AT(C_00833908**, g_pCurrentScene, 0x10)[index];
        if (!object)
            continue;

        const uint32_t initialType = object->field_0050;
        if (initialType != 26 && initialType != 27)
            continue;

        const int regionKey = static_cast<int>(object->field_0020);
        void* regionService = reinterpret_cast<void*>(GetValue8B0554());
        const int regionIndex =
            SceneLoader_ResolveRegionIndex(regionService, nullptr, regionKey);

        regionService = reinterpret_cast<void*>(GetValue8B0554());
        void* region = reinterpret_cast<void*>(
            GetSceneRegionAt(regionService, nullptr, regionIndex));

        void* globalService = reinterpret_cast<void*>(GetGlobalInt());
        void* resourceManager = reinterpret_cast<void*>(
            SceneLoader_GetResourceManager(globalService, nullptr));

        int resourceType = 0;
        switch (object->field_0050)
        {
        case 26:
            resourceType = 1;
            break;
        case 27:
            resourceType = 2;
            break;
        }

        const char* regionName;
        if (CSF_AT(uint32_t, region, 0x20) < 0x10u)
            regionName = reinterpret_cast<const char*>(region) + 0xC;
        else
            regionName = CSF_AT(const char*, region, 0xC);

        const int resourceIndex =
            reinterpret_cast<int(__fastcall*)(void*, void*, int, const char*)>(
                CSF_AT(void**, resourceManager, 0)[0x50 / 4])(
                    resourceManager, nullptr, resourceType, regionName);

        const int resourceAddress =
            reinterpret_cast<int(__fastcall*)(void*, void*, int, int)>(
                CSF_AT(void**, resourceManager, 0)[0x4C / 4])(
                    resourceManager, nullptr, resourceType, resourceIndex);

        SceneLoader_AssignObjectResource(
            object, nullptr, static_cast<uint32_t>(resourceAddress), 0);

        void* resource = reinterpret_cast<void*>(resourceAddress);
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, resource, 0)[0x34 / 4])(resource, nullptr, 1);
    }

    const uint32_t sceneEntry =
        CSF_AT(uint32_t, self, 0x30) + 88 * static_cast<uint32_t>(sceneIndex);
    CSF_AT(uint32_t, reinterpret_cast<void*>(sceneEntry), 0x50) = 0;
    return static_cast<int>(sceneEntry);
}

HOOK(0x004FF9B0, CSceneLoader_InitializeRegionResources, "auto");
