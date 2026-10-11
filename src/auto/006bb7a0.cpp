#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(uint32_t*, __cdecl, RwStreamReadInt32, 0x00629C90, (uint32_t* a1, void* a2, unsigned int a3));
CSF_FUNC(int, __fastcall, GfxObjectProps_InternProperties, 0x006B9510, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GfxObjectProps_ReleaseProperties, 0x006B9E10, (void* self, void* edx, int* a2));
CSF_FUNC(uint32_t*, __fastcall, GfxObjectProps_CreateProperties, 0x006BA460, (void* self, void* edx));
CSF_FUNC(void*, __cdecl, GfxObjectProps_ReadPropertyData, 0x006BA610, (uint32_t* a1));

CSF_GLOBAL(int, g_MaxPropertiesVersion, 0x008A7E20);
CSF_GLOBAL(int, g_MinPropertiesVersion, 0x008A7E34);
CSF_GLOBAL(void*, g_pRwEngineInstance, 0x008B68C0);
CSF_GLOBAL(int, g_PropertiesPluginOffset, 0x008BB474);
CSF_GLOBAL(void*, g_EntryTable, 0x008BB480);

static int __cdecl GfxObjectProps_ReadObjectProperties(
    uint32_t* stream, int /*unused*/, int entryOffset)
{
    int propertiesHandle = CSF_AT(int, g_EntryTable, entryOffset + 4);
    if (propertiesHandle)
    {
        GfxObjectProps_ReleaseProperties(
            CSF_AT(void*, g_pRwEngineInstance, g_PropertiesPluginOffset + 8),
            nullptr, &propertiesHandle);
    }
    CSF_AT(int, g_EntryTable, entryOffset + 4) = propertiesHandle;

    int versionOrPresence = 0;
    RwStreamReadInt32(stream, &versionOrPresence, 4);
    if (versionOrPresence < g_MinPropertiesVersion ||
        versionOrPresence > g_MaxPropertiesVersion)
    {
        return 0;
    }

    RwStreamReadInt32(stream, &versionOrPresence, 4);
    if (versionOrPresence)
    {
        uint32_t* properties = GfxObjectProps_CreateProperties(
            CSF_AT(void*, g_pRwEngineInstance, g_PropertiesPluginOffset + 8),
            nullptr);
        int temporaryHandle = reinterpret_cast<int>(properties);

        int propertyFlags = 0;
        RwStreamReadInt32(stream, &propertyFlags, 4);
        CSF_AT(int, properties, 4) = propertyFlags;
        CSF_AT(uint32_t, properties, 0) =
            reinterpret_cast<uint32_t>(GfxObjectProps_ReadPropertyData(stream));

        int pluginOffset = g_PropertiesPluginOffset;
        propertiesHandle = GfxObjectProps_InternProperties(
            CSF_AT(void*, g_pRwEngineInstance, pluginOffset + 8),
            nullptr, reinterpret_cast<int>(properties));

        GfxObjectProps_ReleaseProperties(
            CSF_AT(void*, g_pRwEngineInstance, pluginOffset + 8),
            nullptr, &temporaryHandle);

        CSF_AT(int, g_EntryTable, entryOffset + 4) = propertiesHandle;
    }

    return reinterpret_cast<int>(stream);
}

HOOK(0x006BB7A0, GfxObjectProps_ReadObjectProperties, "auto");
