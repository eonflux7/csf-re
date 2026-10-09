#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, GetSceneRegionAt, 0x004FC130, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t, __fastcall, SceneLoader_ApplyRegionEntry, 0x004ED1C0, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, SceneLoader_AdvanceRegionIndex, 0x004E9D50, (void* self, void* edx, int a2));

static int __cdecl SceneLoader_ProcessAllRegionEntries()
{
    // First read: the object at +0x1FC8 (region table object used for virtual dispatch).
    uint8_t* sceneBase = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(GetValue8B0554()));
    void* regionTableObj = reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(uint32_t, sceneBase, 0x1FC8)));

    // Second read: the region count at +0xC.
    int result = GetValue8B0554();
    uint32_t regionCount = CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(result)), 0xC);

    for (uint32_t i = 0; i < regionCount; ++i)
    {
        void* regionOwner = reinterpret_cast<void*>(static_cast<uintptr_t>(GetValue8B0554()));
        int regionPtr = GetSceneRegionAt(regionOwner, nullptr, static_cast<int>(i));
        uint32_t regionFirstDword = CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(regionPtr)), 0);

        // Virtual call: vtable slot 4 (byte offset 0x10) of regionTableObj, __thiscall with one stack argument.
        auto virtualRegionFlag = reinterpret_cast<int (__fastcall*)(void*, void*, int)>(CSF_AT(void**, regionTableObj, 0)[4]);
        int regionFlagsValue = virtualRegionFlag(regionTableObj, nullptr, static_cast<int>(regionFirstDword));

        void* entryOwner = reinterpret_cast<void*>(static_cast<uintptr_t>(GetValue8B0554()));
        SceneLoader_ApplyRegionEntry(entryOwner, nullptr, regionFlagsValue, 0);

        void* advanceOwner = reinterpret_cast<void*>(static_cast<uintptr_t>(GetValue8B0554()));
        result = SceneLoader_AdvanceRegionIndex(advanceOwner, nullptr, static_cast<int>(i));
    }

    return result;
}

HOOK(0x004FEDB0, SceneLoader_ProcessAllRegionEntries, "auto");
