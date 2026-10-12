#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, RenderHandle_FromObject, 0x006B2D50, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_00860bb8_GetEntryByIndex, 0x006CB8B0, (C_00860bb8* self, void* /*edx*/, int index));
CSF_FUNC(void, __fastcall, C_00863044_ProcessEntry, 0x006FF2E0, (void* self, void* edx, uint32_t* a2, int a3));

static char __fastcall C_00863044_ProcessSceneEntries(C_00863044* self, void* /*edx*/, int a3)
{
    char result = static_cast<char>(self->field_0022);
    if (result != 0)
    {
        auto* globalInt = reinterpret_cast<C_0085e7b8*>(GetGlobalInt());
        auto* renderHandle = reinterpret_cast<C_00860bb8*>(RenderHandle_FromObject(globalInt, nullptr));
        uint32_t sceneCount = static_cast<uint32_t>(SceneLoader_GetSceneController(renderHandle, nullptr));
        result = static_cast<char>(sceneCount);

        for (uint32_t entryIndex = 0; entryIndex < sceneCount; ++entryIndex)
        {
            void* entry = reinterpret_cast<void*>(
                C_00860bb8_GetEntryByIndex(renderHandle, nullptr, static_cast<int>(entryIndex)));

            if (((uint8_t(__fastcall*)(void*, void*))CSF_AT(void**, entry, 0)[10])(entry, nullptr)
                && ((uint8_t(__fastcall*)(void*, void*))CSF_AT(void**, entry, 0)[12])(entry, nullptr)
                && ((uint8_t(__fastcall*)(void*, void*))CSF_AT(void**, entry, 0)[13])(entry, nullptr))
            {
                C_00863044_ProcessEntry(self, nullptr, reinterpret_cast<uint32_t*>(entry), a3);
            }

            result = static_cast<char>(sceneCount);
        }
    }
    return result;
}

HOOK(0x006FF390, C_00863044_ProcessSceneEntries, "auto");
