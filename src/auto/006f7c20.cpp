#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, RpHAnimHierarchyDestroy, 0x0056CD30, (uint32_t* a1));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));
CSF_FUNC(int, __cdecl, HAnimForwardToRwEngineSlot67, 0x006E83D0, (int a1));

static int __fastcall GfxMaterials_ClearAnimationResources(void* self, void* /*edx*/)
{
    for (uint32_t entryIndex = 0;
         entryIndex < CSF_AT(uint32_t, self, 0x08);
         ++entryIndex)
    {
        RpHAnimHierarchyDestroy(
            CSF_AT(uint32_t*, CSF_AT(void**, self, 0x00)[entryIndex], 0x00));

        for (uint32_t groupIndex = 0;
             groupIndex < CSF_AT(uint32_t,
                 CSF_AT(void**, self, 0x00)[entryIndex], 0x14);
             ++groupIndex)
        {
            void* group = CSF_AT(void**,
                CSF_AT(void**, self, 0x00)[entryIndex], 0x0C)[groupIndex];

            for (uint32_t itemIndex = 0;
                 itemIndex < CSF_AT(uint32_t, group, 0x08);
                 ++itemIndex)
            {
                RpHAnimHierarchyDestroy(
                    CSF_AT(uint32_t*,
                        CSF_AT(void**, group, 0x00)[itemIndex], 0x08));
                RpHAnimHierarchyDestroy(
                    CSF_AT(uint32_t*,
                        CSF_AT(void**, group, 0x00)[itemIndex], 0x0C));

                void* item = CSF_AT(void**, group, 0x00)[itemIndex];
                if (item)
                {
                    CoreMemory_Free(item);
                    CSF_AT(void**, group, 0x00)[itemIndex] = nullptr;
                }
            }

            void* items = CSF_AT(void*, group, 0x00);
            if (items)
            {
                CoreMemory_Free(items);
                CSF_AT(void*, group, 0x00) = nullptr;
            }
            CoreMemory_Free(group);
        }

        HAnimForwardToRwEngineSlot67(
            CSF_AT(int, CSF_AT(void**, self, 0x00)[entryIndex], 0x08));

        void* entry = CSF_AT(void**, self, 0x00)[entryIndex];
        if (entry)
        {
            void* groups = CSF_AT(void*, entry, 0x0C);
            if (groups)
            {
                CoreMemory_Free(groups);
                CSF_AT(void*, entry, 0x0C) = nullptr;
            }
            CoreMemory_Free(entry);
            CSF_AT(void**, self, 0x00)[entryIndex] = nullptr;
        }
    }

    void* entries = CSF_AT(void*, self, 0x00);
    CSF_AT(uint32_t, self, 0x08) = 0;
    if (entries)
    {
        int result = CoreMemory_Free(entries);
        CSF_AT(void*, self, 0x00) = nullptr;
        return result;
    }
    return 0;
}

HOOK(0x006F7C20, GfxMaterials_ClearAnimationResources, "auto");
