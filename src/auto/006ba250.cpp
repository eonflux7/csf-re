#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(char, __fastcall, GfxObjectProps_ClearPropertyEntries, 0x006B9620, (void* self, void* edx));
CSF_FUNC(char, __fastcall, GfxObjectProps_ClearSecondaryEntries, 0x006B9C10, (void* self, void* edx));
CSF_FUNC(char, __fastcall, GfxObjectProps_ClearTertiaryEntries, 0x006B9D80, (void* self, void* edx));

CSF_GLOBAL(float, g_Float_8A7E40, 0x008A7E40);
CSF_GLOBAL(void*, g_pRwEngineInstance, 0x008B68C0);
CSF_GLOBAL(int, g_StreamingState, 0x008BB470);
CSF_GLOBAL(int, g_StreamingPluginOffset, 0x008BB474);

static int __cdecl GfxObjectProps_ResetStreamingState()
{
    auto* plugin = static_cast<uint8_t*>(g_pRwEngineInstance) +
                   g_StreamingPluginOffset;

    void* secondaryEntries = CSF_AT(void*, plugin, 0x04);
    void* tertiaryEntries = CSF_AT(void*, plugin, 0x08);
    void* propertyEntries = CSF_AT(void*, plugin, 0x00);

    // The original's preliminary loops have no side effects.
    GfxObjectProps_ClearTertiaryEntries(tertiaryEntries, nullptr);
    GfxObjectProps_ClearSecondaryEntries(secondaryEntries, nullptr);

    // Preserve the full EAX result: the supplied implementation confirms
    // this callee returns int, despite its packet binding declaring char.
    using ClearEntriesFunction = int (__fastcall*)(void*, void*);
    const int result =
        reinterpret_cast<ClearEntriesFunction>(
            GfxObjectProps_ClearPropertyEntries)(propertyEntries, nullptr);

    g_StreamingState = 0;
    g_Float_8A7E40 = 1.0f;
    return result;
}

HOOK(0x006BA250, GfxObjectProps_ResetStreamingState, "auto");
