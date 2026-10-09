#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetSceneRegionAt, 0x004FC130, (void* self, void* edx, int a2));
CSF_FUNC(bool, __cdecl, sub_7559A0, 0x007559A0, (int a1, int a2));

CSF_GLOBAL(int, g_Value8B0554, 0x008B0554);

namespace {
// Two-byte option block the original builds in the argument slot and passes by address.
struct RegionEntryFlags
{
    uint8_t enabled;   // result of sub_7559A0
    uint8_t reserved;  // always 0
};
}

static int __fastcall C_0085e7b8_RegisterSceneRegionEntries(C_0085e7b8* self, void* /*edx*/, int regionIndex)
{
    uint8_t* region = reinterpret_cast<uint8_t*>(
        GetSceneRegionAt(reinterpret_cast<void*>(g_Value8B0554), nullptr, regionIndex));

    // vector of 12-byte entries: begin at +0x94, end at +0x98
    int entryCount;
    int entriesBegin = CSF_AT(int, region, 0x94);
    if (entriesBegin != 0)
        entryCount = (CSF_AT(int, region, 0x98) - entriesBegin) / 12;
    else
        entryCount = 0;

    void* sceneProvider = CSF_AT(void*, reinterpret_cast<void*>(g_Value8B0554), 0x1FC8);
    int regionKey = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, sceneProvider, 0)[0x10 / 4])(
        sceneProvider, nullptr, CSF_AT(int, region, 0));

    void* ownProvider = CSF_AT(void*, self, 0x1FC8);
    uint8_t* regionInfo = ((uint8_t*(__fastcall*)(void*, void*, int))CSF_AT(void**, ownProvider, 0)[8 / 4])(
        ownProvider, nullptr, regionKey);

    RegionEntryFlags flags;
    flags.enabled = 0;
    flags.reserved = 0;

    bool enabled = sub_7559A0(CSF_AT(int, regionInfo, 0xB4), CSF_AT(int, regionInfo, 0xB8));
    int result = static_cast<int>(enabled);
    flags.enabled = static_cast<uint8_t>(enabled);

    if (entryCount != 0)
    {
        int entryOffset = 0;
        do
        {
            void* registry = CSF_AT(void*, self, 0x1FC4);
            int entryValue = CSF_AT(int, reinterpret_cast<uint8_t*>(CSF_AT(int, region, 0x94)) + entryOffset, 8);
            result = ((int(__fastcall*)(void*, void*, int, void*))CSF_AT(void**, registry, 0)[0x1C / 4])(
                registry, nullptr, entryValue, &flags);
            entryOffset += 12;
            --entryCount;
        } while (entryCount != 0);
    }
    return result;
}

HOOK(0x004E9D50, C_0085e7b8_RegisterSceneRegionEntries, "auto");
