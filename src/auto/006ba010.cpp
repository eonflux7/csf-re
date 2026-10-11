#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pRwEngineInstance, 0x008B68C0);
CSF_GLOBAL(int, g_LastUserDataEntry, 0x008BB470);
CSF_GLOBAL(int, g_UserDataEntryOffset, 0x008BB474);

CSF_FUNC(int, __fastcall, sub_6B9CB0, 0x006B9CB0, (void* self, void* edx, int* a2));

static int __cdecl GfxObjectProps_HandleUserDataEntry(int entryId)
{
    const bool isSameAsLast = (entryId == g_LastUserDataEntry);
    void* rwEngine = g_pRwEngineInstance;
    S_006b9130* holder = *reinterpret_cast<S_006b9130**>(
        reinterpret_cast<uint8_t*>(rwEngine) + g_UserDataEntryOffset + 4);

    int result = sub_6B9CB0(holder, rwEngine, &entryId);

    if (isSameAsLast && result == 0)
        g_LastUserDataEntry = 0;
    return result;
}

HOOK(0x006BA010, GfxObjectProps_HandleUserDataEntry, "auto");
