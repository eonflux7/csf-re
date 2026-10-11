#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"

// Callee: releases an entry through the engine object (__fastcall, one stack arg, ret 4).
CSF_FUNC(int, __fastcall, ReleaseEngineEntry, 0x006B9E10, (void* self, void* edx, int* a2));

CSF_GLOBAL(void*, g_EntryTable, 0x008BB480);
CSF_GLOBAL(void*, g_pRwEngineInstance, 0x008B68C0);
CSF_GLOBAL(int, g_RwEngineObjectOffset, 0x008BB474);

// Takes the entry index, hands the entry's stored value to the engine's release routine,
// clears the slot, and returns the index unchanged.
static int __cdecl ReleaseEntryTableSlot(int entryIndex)
{
    int entryValue = CSF_AT(int, g_EntryTable, entryIndex + 4);

    void* engineObject = CSF_AT(void*, g_pRwEngineInstance, g_RwEngineObjectOffset + 8);
    ReleaseEngineEntry(engineObject, nullptr, &entryValue);

    CSF_AT(int, g_EntryTable, entryIndex + 4) = 0;
    return entryIndex;
}

HOOK(0x006BA170, ReleaseEntryTableSlot, "auto");
