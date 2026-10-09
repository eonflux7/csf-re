#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GlobalTableHasEntry, 0x004FC140, (void* self, void* edx, unsigned int a2));
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void*));
CSF_FUNC(int, __cdecl, RpWorldRemoveCamera, 0x0058DDC0, (int a1, int a2));
CSF_FUNC(int, __cdecl, RwCameraGetWorld, 0x0058DDF0, (int a1));
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneObject, 0x00652010, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());

static int __cdecl SceneLoader_DetachCameras()
{
    void* sceneObject = reinterpret_cast<void*>(
        SceneLoader_GetSceneObject(reinterpret_cast<void*>(GetGlobalInt()), nullptr));
    void* worldProvider =
        (reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, sceneObject, 0)[0x40 / 4]))(sceneObject, nullptr);
    int world = reinterpret_cast<int>(GetReferencedObject(worldProvider, nullptr));

    int firstEntry = GetFirstElement(
        reinterpret_cast<void*>(GetGlobalValue_4FBDE0()), nullptr);

    unsigned int index = 0;
    int table = GetGlobalValue_4FBDE0();
    if (CSF_AT(std::uint32_t, reinterpret_cast<void*>(table), 0x1C) != 0)
    {
        do
        {
            int entry = GlobalTableHasEntry(
                reinterpret_cast<void*>(GetGlobalValue_4FBDE0()), nullptr, index);
            void* object = reinterpret_cast<void*>(
                ResolveEntryLookupValue(reinterpret_cast<void*>(entry), nullptr));
            void* cameraProvider =
                (reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                    CSF_AT(void**, object, 0)[0x14 / 4]))(object, nullptr);
            int camera = reinterpret_cast<int>(
                GetCachedObjectProvider(cameraProvider, nullptr));

            if (RwCameraGetWorld(camera) == world)
                RpWorldRemoveCamera(world, camera);

            if (firstEntry != entry)
            {
                object = reinterpret_cast<void*>(
                    ResolveEntryLookupValue(reinterpret_cast<void*>(entry), nullptr));
                (reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, object, 0)[0x44 / 4]))(object, nullptr, 0);
            }

            ++index;
            table = GetGlobalValue_4FBDE0();
        }
        while (index < CSF_AT(std::uint32_t, reinterpret_cast<void*>(table), 0x1C));
    }

    return table;
}

HOOK(0x004FE880, SceneLoader_DetachCameras, "auto");
