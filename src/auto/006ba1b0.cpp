#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pRwEngineInstance, 0x008B68C0);
CSF_GLOBAL(int, g_ObjectPropertiesPluginOffset, 0x008BB474);
CSF_GLOBAL(void*, g_EntryTable, 0x008BB480);

CSF_FUNC(int, __fastcall, GfxObjectProps_FindSharedEntry, 0x006B9130,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GfxObjectProps_ReleaseSharedEntry, 0x006B9E10,
         (void* self, void* edx, int* a2));

static int __cdecl GfxObjectProps_CopySharedProperties(
    int destinationOffset, int sourceOffset)
{
    // The packet also identifies the lookup address as a vtable reference.
    (void)CSF_VTBL(0x006B9130);

    auto* engine = static_cast<std::uint8_t*>(g_pRwEngineInstance);
    const int pluginOffset = g_ObjectPropertiesPluginOffset;
    auto* entryTable = static_cast<std::uint8_t*>(g_EntryTable);
    auto* sharedEntries =
        CSF_AT(S_006b9130*, engine + pluginOffset, 8);

    const int sourceProperties = CSF_AT(int, entryTable + sourceOffset, 4);
    int oldProperties = CSF_AT(int, entryTable + destinationOffset, 4);

    if (oldProperties != 0)
    {
        GfxObjectProps_ReleaseSharedEntry(
            sharedEntries, nullptr, &oldProperties);
        entryTable = static_cast<std::uint8_t*>(g_EntryTable);
    }

    int copiedProperties = 0;
    if (sourceProperties != 0)
    {
        const int index = GfxObjectProps_FindSharedEntry(
            sharedEntries, nullptr, sourceProperties);
        if (index != -1)
        {
            const std::uint32_t entryOffset =
                static_cast<std::uint32_t>(index) * 8;

            auto* entries =
                reinterpret_cast<std::uint8_t*>(sharedEntries->field_0004);
            CSF_AT(std::uint32_t, entries + entryOffset, 4) += 1;

            entries =
                reinterpret_cast<std::uint8_t*>(sharedEntries->field_0004);
            copiedProperties = CSF_AT(int, entries + entryOffset, 0);
        }
    }

    CSF_AT(int, entryTable + destinationOffset, 4) = copiedProperties;
    return destinationOffset;
}

HOOK(0x006BA1B0, GfxObjectProps_CopySharedProperties, "auto");
