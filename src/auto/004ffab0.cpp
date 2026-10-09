#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int32_t, __cdecl, GetGlobalValue_51F740, 0x0051F740, ());
CSF_FUNC(int, __fastcall, ResolveGlobalEntryToOutput, 0x00520D60, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, LookupEntryByKey, 0x007686E0, (void* self, void* edx, int a2));

static int __cdecl SceneLoader_ResolveMatchingEntries()
{
    void* scene = reinterpret_cast<void*>(GetValue8B0554());
    void* firstLookup = CSF_AT(void*, scene, 0x1FCC);

    scene = reinterpret_cast<void*>(GetValue8B0554());
    auto* entries = reinterpret_cast<uint8_t*>(scene) + 0x14;

    scene = reinterpret_cast<void*>(GetValue8B0554());
    void* secondLookup = CSF_AT(void*, scene, 0x1F90);

    const uint32_t begin = CSF_AT(uint32_t, entries, 0x4);
    uint32_t remaining = 0;
    if (begin != 0)
    {
        const uint32_t end = CSF_AT(uint32_t, entries, 0x8);
        remaining = static_cast<uint32_t>(
            static_cast<int32_t>(end - begin) / 52);
    }

    uint32_t offset = 0;
    while (remaining != 0)
    {
        auto* entry = reinterpret_cast<uint8_t*>(
            CSF_AT(uint32_t, entries, 0x4)) + offset;

        const int firstMatch =
            reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, firstLookup, 0)[0x10 / 4])(
                    firstLookup, nullptr, CSF_AT(int, entry, 0x20));

        const int secondMatch = LookupEntryByKey(
            secondLookup, nullptr, CSF_AT(int, entry, 0x24));

        if (firstMatch != -1 && secondMatch != -1)
        {
            void* resolver = reinterpret_cast<void*>(GetGlobalValue_51F740());
            ResolveGlobalEntryToOutput(
                resolver, nullptr, reinterpret_cast<uint32_t*>(entry));
        }

        offset += 52;
        --remaining;
    }

    return static_cast<int>(remaining);
}

HOOK(0x004FFAB0, SceneLoader_ResolveMatchingEntries, "auto");
