#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int32_t, __cdecl, GetGlobalValue_51F740, 0x0051F740, ());
CSF_FUNC(int, __fastcall, ApplyResolvedBinding, 0x00520F30, (void* self, void* edx, int a2, int a3, int a4));

// Resolves the entity referenced by a3[0] in the current scene, queries its
// component for a track index using the selector in a3[1], and when that index
// is valid, notifies the component and forwards the resolved binding values.
static int __stdcall SceneEntry_ApplyTrackBinding(int /*a1*/, int /*a2*/, uint32_t* a3)
{
    uint32_t entryRecord = CSF_AT(uint32_t, reinterpret_cast<void*>(a3[0]), 4);
    int32_t result = CSF_AT(int32_t, reinterpret_cast<void*>(entryRecord), 12);
    uint32_t handle = entryRecord + 4;

    if (result >= 0)
    {
        int entityHandle = CHandleTable_Resolve(g_pCurrentScene, nullptr, static_cast<int>(handle));
        result = entityHandle;
        if (entityHandle != 0)
        {
            void* entity = reinterpret_cast<void*>(static_cast<uintptr_t>(entityHandle));

            void* component = ((void*(__fastcall*)(void*, void*))CSF_AT(void**, entity, 0)[10])(entity, nullptr);

            uint32_t selectorRecord = CSF_AT(uint32_t, reinterpret_cast<void*>(a3[1]), 4);
            int32_t selectorValue = static_cast<int32_t>(CSF_AT(uint32_t, reinterpret_cast<void*>(selectorRecord), 4));

            int32_t trackIndex = ((int32_t(__fastcall*)(void*, void*, int32_t))CSF_AT(void**, component, 0)[38])(
                component, nullptr, selectorValue);
            result = trackIndex;

            if (trackIndex != -1)
            {
                void* notifyComponent = ((void*(__fastcall*)(void*, void*))CSF_AT(void**, entity, 0)[10])(entity, nullptr);
                ((void(__fastcall*)(void*, void*, int32_t))CSF_AT(void**, notifyComponent, 0)[35])(
                    notifyComponent, nullptr, trackIndex);

                uint32_t thirdRecord = CSF_AT(uint32_t, reinterpret_cast<void*>(a3[2]), 4);
                uint32_t thirdValue = CSF_AT(uint32_t, reinterpret_cast<void*>(thirdRecord), 4);

                uint32_t secondRecord = CSF_AT(uint32_t, reinterpret_cast<void*>(a3[1]), 4);
                uint32_t secondValue = CSF_AT(uint32_t, reinterpret_cast<void*>(secondRecord), 4);

                int32_t globalValue = GetGlobalValue_51F740();

                return ApplyResolvedBinding(
                    reinterpret_cast<void*>(static_cast<uintptr_t>(globalValue)),
                    nullptr,
                    static_cast<int>(handle),
                    static_cast<int>(secondValue),
                    static_cast<int>(thirdValue));
            }
        }
    }
    return result;
}

HOOK(0x460340, SceneEntry_ApplyTrackBinding, "auto");
