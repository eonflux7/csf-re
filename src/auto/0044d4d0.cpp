#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());

static int __fastcall AppendSceneEntryIndex(void* self, void* /*edx*/, int sceneIndex)
{
    void* controller = reinterpret_cast<void*>(
        SceneLoader_GetSceneController(reinterpret_cast<void*>(GetGlobalInt()), nullptr));

    int resolvedIndex =
        ((int(__fastcall*)(void*, void*, int))
            CSF_AT(void**, controller, 0)[0x74 / 4])
        (controller, nullptr, sceneIndex);

    if (resolvedIndex == -1)
        return resolvedIndex;

    controller = reinterpret_cast<void*>(
        SceneLoader_GetSceneController(reinterpret_cast<void*>(GetGlobalInt()), nullptr));

    void* sceneObject =
        ((void*(__fastcall*)(void*, void*, int))
            CSF_AT(void**, controller, 0)[0x70 / 4])
        (controller, nullptr, resolvedIndex);

    ((void(__fastcall*)(void*, void*, int))
        CSF_AT(void**, sceneObject, 0)[0x30 / 4])
        (sceneObject, nullptr, 0);

    ((void(__fastcall*)(void*, void*, int))
        CSF_AT(void**, sceneObject, 0)[0x38 / 4])
        (sceneObject, nullptr, 0);

    if (CSF_AT(uint32_t, self, 0x10) >= CSF_AT(uint32_t, self, 0x0C))
    {
        (void)CSF_VTBL(0x0041E5F0);
        S_0041e5f0_GrowArrayCapacity(
            reinterpret_cast<S_0041e5f0*>(reinterpret_cast<uint8_t*>(self) + 8),
            nullptr,
            CSF_AT(int, self, 0x14));
    }

    uint32_t count = CSF_AT(uint32_t, self, 0x10);
    uint32_t* destination =
        reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, 0x08)) + count;
    uint32_t newCount = count + 1;
    CSF_AT(uint32_t, self, 0x10) = newCount;
    *destination = static_cast<uint32_t>(sceneIndex);

    return static_cast<int>(newCount);
}

HOOK(0x0044D4D0, AppendSceneEntryIndex, "auto");
