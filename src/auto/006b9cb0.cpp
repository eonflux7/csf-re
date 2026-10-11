#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __fastcall, S_006b9130_FindEntryIndex, 0x006B9130,
         (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, S_006b9130_RemoveEntry, 0x006B95E0,
         (void* self, void* edx, uint32_t* a2, uint32_t* a3));
CSF_FUNC(int, __fastcall, S_006b9130_ReleaseNestedReference, 0x006B96C0,
         (void* self, void* edx, int* a2));

CSF_GLOBAL(void*, g_pRwEngineInstance, 0x008B68C0);
CSF_GLOBAL(uint32_t, g_ObjectPropertiesPluginOffset, 0x008BB474);

static int __fastcall S_006b9130_ReleaseProperty(
    S_006b9130* self, void* /*edx*/, int* handle)
{
    int* originalHandle = handle;
    const int index = S_006b9130_FindEntryIndex(self, nullptr, *handle);
    if (index == -1)
    {
        *originalHandle = 0;
        return -1;
    }

    const uint32_t entryOffset = 8 * static_cast<uint32_t>(index);
    auto* entry = reinterpret_cast<uint8_t*>(self->field_0004) + entryOffset;
    CSF_AT(uint32_t, entry, 4) = CSF_AT(uint32_t, entry, 4) - 1;

    entry = reinterpret_cast<uint8_t*>(self->field_0004) + entryOffset;
    const int remainingReferences = CSF_AT(int, entry, 4);
    if (remainingReferences != 0)
    {
        *originalHandle = 0;
        return remainingReferences;
    }

    void* property = CSF_AT(void*, entry, 0);
    if (property)
    {
        const uint32_t pluginOffset = g_ObjectPropertiesPluginOffset;
        CSF_AT(uint8_t, property, 1) = 0;
        CSF_AT(uint8_t, property, 0) = 0;
        CSF_AT(int, property, 8) = -1;

        S_006b9130_ReleaseNestedReference(
            CSF_AT(S_006b9130*, g_pRwEngineInstance, pluginOffset),
            nullptr,
            reinterpret_cast<int*>(static_cast<uint8_t*>(property) + 4));

        CSF_AT(uint8_t, property, 0x11) = 0;
        Mem_Free(property);

        entry = reinterpret_cast<uint8_t*>(self->field_0004) + entryOffset;
        CSF_AT(uint32_t, entry, 0) = 0;
    }

    entry = reinterpret_cast<uint8_t*>(self->field_0004) + entryOffset;
    S_006b9130_RemoveEntry(
        self, nullptr, reinterpret_cast<uint32_t*>(&handle),
        reinterpret_cast<uint32_t*>(entry));

    *originalHandle = 0;
    return remainingReferences;
}

HOOK(0x006B9CB0, S_006b9130_ReleaseProperty, "auto");
