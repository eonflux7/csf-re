#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwStreamClose, 0x00629810, (uint32_t* a1, uint32_t* a2));
CSF_FUNC(uint32_t*, __cdecl, RwStreamOpen, 0x006298E0, (int a3, int a4, int a5));
CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));
CSF_FUNC(int, __fastcall, C_00863ba8_FindMatchingEntryIndex, 0x0070F080, (C_00863ba8* self, void* , int key));
CSF_FUNC(void, __cdecl, _qsort, 0x007AEAE0, (void* base, std::size_t count, std::size_t size, int (__cdecl* compare)(const void*, const void*)));
CSF_FUNC(int, __cdecl, RtDictGetEntry, 0x007DBD70, (int a1, int a2));
CSF_FUNC(int, __cdecl, RtDictGetNumEntries, 0x007DBD80, (int a1));
CSF_FUNC(int, __cdecl, Audio_GetEffectsDictionary, 0x007DBE90, ());
CSF_FUNC(uint32_t*, __cdecl, Audio_WriteEffectEntry, 0x007E0200, (uint32_t* a1, uint32_t* a2, uint32_t* a3, uint32_t* a4, int a5));

CSF_GLOBAL(uint32_t, g_AudioEntryMappings, 0x008A85A8);
CSF_GLOBAL(uint8_t, g_AudioFormatFlags, 0x008A85B0);
CSF_GLOBAL(uint32_t, g_AudioFormatValues, 0x008A85B4);

static char __fastcall C_00863ba8_WriteEffectsDictionary(
    C_00863ba8* self, void* /*edx*/, int streamSource, int formatIndex)
{
    if (!Audio_GetEffectsDictionary())
        return 1;

    const uint8_t formatFlag =
        CSF_AT(uint8_t, &g_AudioFormatFlags, 16 * formatIndex);
    const uint32_t formatValue =
        CSF_AT(uint32_t, &g_AudioFormatValues, 16 * formatIndex);

    alignas(8) uint8_t optionsBytes[0x28] = {};
    CSF_AT(uint32_t, optionsBytes, 0x00) = 0;
    CSF_AT(uint32_t, optionsBytes, 0x08) = 0;
    CSF_AT(uint8_t, optionsBytes, 0x0D) = 0;
    CSF_AT(uint8_t, optionsBytes, 0x18) = 0;
    CSF_AT(uint8_t, optionsBytes, 0x19) = 0;
    CSF_AT(uint32_t, optionsBytes, 0x10) = 0;
    CSF_AT(uint32_t, optionsBytes, 0x14) = 0;
    CSF_AT(uint8_t, optionsBytes, 0x0C) = formatFlag;
    CSF_AT(uint32_t, optionsBytes, 0x04) = formatValue;

    uint32_t* stream = RwStreamOpen(2, 2, streamSource);
    if (!stream)
        return 0;

    const int entryCount = RtDictGetNumEntries(Audio_GetEffectsDictionary());
    uint32_t* entries = reinterpret_cast<uint32_t*>(
        CoreMemory_AllocUntagged(static_cast<std::size_t>(entryCount) * 4));

    for (int index = 0; index < entryCount; ++index)
        entries[index] = RtDictGetEntry(Audio_GetEffectsDictionary(), index);

    _qsort(
        entries, static_cast<std::size_t>(entryCount), 4,
        reinterpret_cast<int (__cdecl*)(const void*, const void*)>(
            CSF_ADDR_FN(0x0070F990)));

    char success = 1;
    for (int index = 0; index < entryCount; ++index)
    {
        uint32_t* entry = reinterpret_cast<uint32_t*>(entries[index]);
        if (!entry)
            continue;

        const int matchingIndex = C_00863ba8_FindMatchingEntryIndex(
            self, nullptr, CSF_AT(int, entry, 0x2C));

        CSF_AT(uint32_t, optionsBytes, 0x00) =
            CSF_AT(uint32_t, &g_AudioEntryMappings,
                   4 * (4 * formatIndex + matchingIndex));

        CSF_AT(uint8_t, optionsBytes, 0x0D) =
            formatIndex == 1 ? 1 : CSF_AT(uint8_t, entry, 0x39);

        uint32_t* options = reinterpret_cast<uint32_t*>(optionsBytes);
        if (!Audio_WriteEffectEntry(entry, stream, options, options, 0))
        {
            success = 0;
            break;
        }
    }

    if (entries)
        CoreMemory_Free(entries);
    RwStreamClose(stream, nullptr);
    return success;
}

HOOK(0x0070F9C0, C_00863ba8_WriteEffectsDictionary, "auto");
