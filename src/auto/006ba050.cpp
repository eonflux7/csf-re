#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, GfxObjectProps_UnregisterEntryKey, 0x006B9CB0, (void* self, void* edx, int* a2));

CSF_GLOBAL(void*, g_EntryTable, 0x008BB47C);
CSF_GLOBAL(void*, g_pRwEngineInstance, 0x008B68C0);
CSF_GLOBAL(int, g_ActiveEntryKey, 0x008BB470);
CSF_GLOBAL(int, g_RwEngineInstanceOffset, 0x008BB474);

// Clears the property slot at entryOffset in g_EntryTable. If that slot's key is the
// active key, the RW-side unregister call runs first and the active key is reset on success.
static int __cdecl GfxObjectProps_ReleaseEntryAt(int entryOffset)
{
    int entryKey = CSF_AT(int, g_EntryTable, entryOffset + 4);
    bool isActiveKey = (entryKey == g_ActiveEntryKey);

    void* rwObject = CSF_AT(void*, g_pRwEngineInstance, g_RwEngineInstanceOffset + 4);
    int unregisterResult = GfxObjectProps_UnregisterEntryKey(rwObject, nullptr, &entryKey);

    if (isActiveKey) {
        CSF_AT(int, g_EntryTable, entryOffset + 4) = 0;
        if (unregisterResult == 0) {
            g_ActiveEntryKey = 0;
        }
        return entryOffset;
    }

    CSF_AT(int, g_EntryTable, entryOffset + 4) = 0;
    return entryOffset;
}

HOOK(0x006BA050, GfxObjectProps_ReleaseEntryAt, "auto");
