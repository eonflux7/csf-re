#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, GetGlobalObjectField184, 0x00475470, ());
CSF_FUNC(void, __fastcall, SceneLoader_RemoveObjectEntry, 0x00475480, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());

static void __fastcall Scene_ResetTrackedObjects(void* self, void* /*edx*/)
{
    SceneLoader_GetSceneController(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);

    for (uint32_t i = 0; i < CSF_AT(uint32_t, self, 0x10); ++i)
    {
        void* controller = reinterpret_cast<void*>(
            SceneLoader_GetSceneController(
                reinterpret_cast<void*>(GetGlobalInt()), nullptr));

        uint32_t identifier =
            reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, 0x08))[i];

        int index =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t)>(
                CSF_AT(void**, controller, 0)[0x74 / 4])(
                    controller, nullptr, identifier);

        if (index != -1)
        {
            controller = reinterpret_cast<void*>(
                SceneLoader_GetSceneController(
                    reinterpret_cast<void*>(GetGlobalInt()), nullptr));

            void* object =
                reinterpret_cast<void*(__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, controller, 0)[0x70 / 4])(
                        controller, nullptr, index);

            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, object, 0)[0x30 / 4])(
                    object, nullptr, 1);

            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, object, 0)[0x38 / 4])(
                    object, nullptr, 1);
        }
    }

    void* buffer = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x08));
    CSF_AT(uint32_t, self, 0x0C) = 0;
    CSF_AT(uint32_t, self, 0x10) = 0;

    if (buffer)
    {
        Mem_Free(buffer);
        CSF_AT(uint32_t, self, 0x08) = 0;
    }

    void* registry = reinterpret_cast<void*>(GetGlobalObjectField184());
    SceneLoader_RemoveObjectEntry(registry, nullptr, reinterpret_cast<int>(self));
}

HOOK(0x0044D3F0, Scene_ResetTrackedObjects, "auto");
