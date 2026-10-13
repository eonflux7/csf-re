#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));

// Clears the streaming-object handle table of a scene object.
// Each non-zero handle is released through the scene controller's vtable slot 6
// (offset 0x18), then zeroed. Returns the handle-table count (offset 0x20).
static uint32_t __fastcall GfxScene_ResetStreamingObject(C_008621c8* self, void* /*edx*/)
{
    using ReleaseHandleFn = void(__fastcall*)(void* self, void* edx, uint32_t handle);

    CSF_AT(uint8_t, self, 0x3c) = 0;

    uint32_t result = CSF_AT(uint32_t, self, 0x20);
    uint32_t index = 0;
    if (result != 0)
    {
        do
        {
            if (CSF_AT(uint32_t, self, 0x24 + index * 4) != 0)
            {
                auto* globalInt = reinterpret_cast<C_00860bb8*>(static_cast<uintptr_t>(GetGlobalInt()));
                void* sceneController = reinterpret_cast<void*>(
                    static_cast<uintptr_t>(SceneLoader_GetSceneController(globalInt, nullptr)));

                auto releaseHandle = reinterpret_cast<ReleaseHandleFn>(
                    CSF_AT(void**, sceneController, 0)[24 / 4]);
                releaseHandle(sceneController, nullptr, CSF_AT(uint32_t, self, 0x24 + index * 4));

                CSF_AT(uint32_t, self, 0x24 + index * 4) = 0;
            }
            result = CSF_AT(uint32_t, self, 0x20);
            ++index;
        } while (index < result);
    }
    return result;
}

HOOK(0x006EF440, GfxScene_ResetStreamingObject, "auto");
