#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(uint32_t*, __cdecl, RwStreamReadInt32, 0x00629C90, (uint32_t* a1, void* a2, unsigned int a3));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __fastcall, GfxObjectProps_RegisterProperties, 0x006B9510, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GfxObjectProps_ReleaseEntry, 0x006B9CB0, (void* self, void* edx, int* a2));
CSF_FUNC(int, __cdecl, GfxObjectProps_ReleaseProperties, 0x006BA010, (int a2));
CSF_FUNC(uint32_t*, __fastcall, GfxObjectProps_CreateProperties, 0x006BA400, (void* self, void* edx));
CSF_FUNC(void*, __cdecl, GfxObjectProps_ReadMaterial, 0x006BA610, (uint32_t* a1));

CSF_GLOBAL(int, g_MaxPropertiesVersion, 0x008A7E1C);
CSF_GLOBAL(int, g_MinPropertiesVersion, 0x008A7E30);
CSF_GLOBAL(void*, g_pRwEngineInstance, 0x008B68C0);
CSF_GLOBAL(int, g_CurrentPropertiesEntry, 0x008BB470);
CSF_GLOBAL(int, g_PropertiesPluginOffset, 0x008BB474);
CSF_GLOBAL(void*, g_EntryTable, 0x008BB47C);

static int __cdecl GfxObjectProps_ReadProperties(
    int streamHandle, int /*unused*/, int entryOffset)
{
    void* manager = CSF_AT(
        void*, g_pRwEngineInstance, g_PropertiesPluginOffset + 4);

    const int oldEntry = CSF_AT(int, g_EntryTable, entryOffset + 4);
    if (oldEntry)
    {
        int entryToRelease = oldEntry;
        const bool wasCurrent = oldEntry == g_CurrentPropertiesEntry;
        const int remaining = GfxObjectProps_ReleaseEntry(
            manager, nullptr, &entryToRelease);
        if (wasCurrent && !remaining)
            g_CurrentPropertiesEntry = 0;

        CSF_AT(int, g_EntryTable, entryOffset + 4) = oldEntry;
    }

    auto* stream = reinterpret_cast<uint32_t*>(streamHandle);
    int version = 0;
    RwStreamReadInt32(stream, &version, 4);
    if (version < g_MinPropertiesVersion || version > g_MaxPropertiesVersion)
        return 0;

    int hasProperties = 0;
    RwStreamReadInt32(stream, &hasProperties, 4);
    if (hasProperties)
    {
        uint32_t* properties = GfxObjectProps_CreateProperties(
            CSF_AT(void*, g_pRwEngineInstance, g_PropertiesPluginOffset + 4),
            nullptr);

        const int byteCount = 4 * ((version >= 2) + 4);
        auto* words = static_cast<uint32_t*>(Mem_Alloc(byteCount));
        RwStreamReadInt32(stream, words, byteCount);

        CSF_AT(uint8_t, properties, 0x1) = words[0] != 0;
        CSF_AT(uint8_t, properties, 0x0) = words[1] != 0;
        CSF_AT(uint32_t, properties, 0xC) |= words[2];
        CSF_AT(uint32_t, properties, 0x8) = words[3];
        if (version >= 2)
            CSF_AT(uint8_t, properties, 0x11) = words[4] != 0;

        Mem_Free(words);
        CSF_AT(void*, properties, 0x4) =
            GfxObjectProps_ReadMaterial(stream);

        const int newEntry = GfxObjectProps_RegisterProperties(
            manager, nullptr, reinterpret_cast<int>(properties));
        GfxObjectProps_ReleaseProperties(reinterpret_cast<int>(properties));
        CSF_AT(int, g_EntryTable, entryOffset + 4) = newEntry;
    }

    return streamHandle;
}

HOOK(0x006BB640, GfxObjectProps_ReadProperties, "auto");
