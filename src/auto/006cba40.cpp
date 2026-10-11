#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SceneLoader_ClearRenderResources, 0x006BDAE0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, SceneLoader_ClearWorldResources, 0x007512A0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, RenderHandle_FromObject, 0x006B2D50, (void* self, void* edx));
CSF_FUNC(void, __fastcall, SceneLoader_SetRenderWorld, 0x006D6A40, (void* self, void* edx, int a2));

static int __fastcall SceneLoader_ReleaseWorldAtIndex(int* self, void* /*edx*/, int index)
{
    // Entry table lives at self+0xA0; if the entry is empty, the original returns the table pointer.
    uint32_t* entryTable = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, 0xA0));
    int result = reinterpret_cast<int>(entryTable);
    uint8_t* entry = reinterpret_cast<uint8_t*>(entryTable[index]);

    if (entry != nullptr)
    {
        // Render resources object at entry+0x38
        if (CSF_AT(uint32_t, entry, 0x38) != 0)
        {
            result = SceneLoader_ClearRenderResources(
                reinterpret_cast<void*>(CSF_AT(uint32_t, entry, 0x38)), nullptr);

            void* renderObj = reinterpret_cast<void*>(CSF_AT(uint32_t, entry, 0x38));
            if (renderObj != nullptr)
            {
                result = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, renderObj, 0)[1])(
                    renderObj, nullptr, 1);
                CSF_AT(uint32_t, entry, 0x38) = 0;
            }
            CSF_AT(uint32_t, entry, 0x38) = 0;
        }

        // World object at entry+0x3C
        if (CSF_AT(uint32_t, entry, 0x3C) != 0)
        {
            SceneLoader_ClearWorldResources(
                reinterpret_cast<void*>(CSF_AT(uint32_t, entry, 0x3C)), nullptr);

            uint32_t worldPtr = CSF_AT(uint32_t, entry, 0x3C);
            void* globalObj = reinterpret_cast<void*>(GetGlobalInt());
            int renderHandle = RenderHandle_FromObject(globalObj, nullptr);
            SceneLoader_SetRenderWorld(reinterpret_cast<void*>(renderHandle), nullptr,
                                       static_cast<int>(worldPtr));
            result = 0;

            void* worldObj = reinterpret_cast<void*>(CSF_AT(uint32_t, entry, 0x3C));
            if (worldObj != nullptr)
            {
                result = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, worldObj, 0)[1])(
                    worldObj, nullptr, 1);
                CSF_AT(uint32_t, entry, 0x3C) = 0;
            }
            CSF_AT(uint32_t, entry, 0x3C) = 0;
        }
    }
    return result;
}

HOOK(0x006CBA40, SceneLoader_ReleaseWorldAtIndex, "auto");
