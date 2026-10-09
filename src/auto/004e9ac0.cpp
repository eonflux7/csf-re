#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void, __fastcall, SceneLoader_SetEntryText, 0x00457770, (void* self, void* edx, void* a2));
CSF_FUNC(int, __fastcall, sub_4F07F0, 0x004F07F0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetSceneRegionAt, 0x004FC130, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(unsigned int, __fastcall, SchedulerBase_GetNumberOfBoundContexts, 0x004FE490, (void* self, void*));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

static uint32_t __fastcall C_0085e7b8_PrepareSceneResources(C_0085e7b8* self, void* /*edx*/)
{
    // 4-byte stack slot (ebp-4). The original only ever writes its first two
    // bytes; bytes [2] and [3] are deliberately left untouched by this function.
    uint8_t loadFlags[4];

    // First pass: request every non-empty resource entry from the resource manager.
    uint32_t entryCount = reinterpret_cast<uint32_t>(
        GetCachedObjectProvider(CSF_AT(void*, self, 0x1fe0), nullptr));

    for (uint32_t entryIndex = 0; entryIndex < entryCount; ++entryIndex)
    {
        int entry = sub_4F07F0(CSF_AT(void*, self, 0x1fe0), nullptr, static_cast<int>(entryIndex));
        if (CSF_AT(uint32_t, entry, 0x14) != 0)
        {
            void* manager = CSF_AT(void*, self, 0x1fc4);
            int resourceKey = CSF_AT(int, entry, 0x1c);
            loadFlags[1] = 0;
            loadFlags[0] = 0;

            int resource = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, manager, 0)[0x8 / 4])(
                manager, nullptr, resourceKey);

            manager = CSF_AT(void*, self, 0x1fc4);
            ((void(__fastcall*)(void*, void*, int, uint8_t*))CSF_AT(void**, manager, 0)[0x1c / 4])(
                manager, nullptr, resource, loadFlags);
        }
    }

    // Second pass: register the name of every scene region.
    uint32_t regionCount = self->field_000c;

    for (uint32_t regionIndex = 0; regionIndex < regionCount; ++regionIndex)
    {
        int region = GetSceneRegionAt(self, nullptr, static_cast<int>(regionIndex));

        // std::string embedded at +0xA0 (buffer/pointer at +4, capacity at +0x18)
        void* regionName;
        if (CSF_AT(uint32_t, region, 0xb8) < 0x10)
            regionName = reinterpret_cast<uint8_t*>(region) + 0xa0 + 4;
        else
            regionName = CSF_AT(void*, region, 0xa0 + 4);

        void* globalOwner = reinterpret_cast<void*>(GetGlobalValue_8B0568());
        void* textTarget = reinterpret_cast<void*>(
            SchedulerBase_GetNumberOfBoundContexts(globalOwner, nullptr));
        SceneLoader_SetEntryText(textTarget, nullptr, regionName);
    }

    return regionCount;
}

HOOK(0x004E9AC0, C_0085e7b8_PrepareSceneResources, "auto");
