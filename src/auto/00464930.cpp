#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, LookupTableIndexOfKey, 0x0046F390, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, QueryEntryObjectFlag, 0x00471BE0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, ProbeEntryObjectState, 0x00672AC0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_0083e500_SetField0008, 0x00713B10, (C_0083e500* self, void* , int value));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall AdvanceEntryCursor(void* self, void* /*edx*/, int /*a2*/, int /*a3*/, uint32_t a4)
{
    // a4 points to a slot holding a node pointer; node+8 is the entry index (bit 23 = scene-table selector).
    uint32_t nodeAddr = *reinterpret_cast<uint32_t*>(a4);
    uint32_t slotIndex = CSF_AT(uint32_t, reinterpret_cast<void*>(nodeAddr), 8);

    uint32_t tableSource = CSF_AT(uint32_t, self, 0x20);
    if ((slotIndex & 0x800000u) != 0)
    {
        uint32_t sceneChild = CSF_AT(uint32_t, g_pCurrentScene, 0xBC);
        tableSource = CSF_AT(uint32_t, reinterpret_cast<void*>(sceneChild), 0x38);
        slotIndex &= ~0x800000u;
    }

    uint32_t tableBase = CSF_AT(uint32_t, reinterpret_cast<void*>(tableSource), 8);
    uint32_t entryAddr = tableBase + 12u * slotIndex;

    int probeResult = ProbeEntryObjectState(
        reinterpret_cast<void*>(CSF_AT(uint32_t, reinterpret_cast<void*>(entryAddr), 4)),
        reinterpret_cast<void*>(3u * slotIndex));

    int result = 0;
    bool flagWasSet;
    if (CSF_AT(uint8_t, self, 0x81) != 0)
    {
        // Only AL is tested by the original; the full EAX stays as the function result.
        result = QueryEntryObjectFlag(
            reinterpret_cast<void*>(CSF_AT(uint32_t, reinterpret_cast<void*>(entryAddr), 4)),
            nullptr);
        flagWasSet = static_cast<uint8_t>(result) != 0;
    }
    else
    {
        flagWasSet = probeResult != 0;
        if (flagWasSet)
        {
            result = C_0083e500_SetField0008(
                reinterpret_cast<C_0083e500*>(CSF_AT(uint32_t, reinterpret_cast<void*>(entryAddr), 4)),
                nullptr,
                0);
        }
    }

    if (!flagWasSet)
    {
        int cursorValue = CSF_AT(int, self, 0x1C);
        int selectedIndex = LookupTableIndexOfKey(CSF_AT(void*, self, 0x10), nullptr, cursorValue);

        void* cellOwner = CSF_AT(void*, self, 0x10);
        uint32_t cellBase = CSF_AT(uint32_t, cellOwner, 0x60);
        uint32_t cellAddr = cellBase + 8u * static_cast<uint32_t>(selectedIndex);
        uint32_t nextCursor = CSF_AT(uint32_t, reinterpret_cast<void*>(cellAddr), 4);

        result = static_cast<int>(cellAddr);
        CSF_AT(uint32_t, self, 0x1C) = nextCursor;
    }

    CSF_AT(uint8_t, self, 0x81) = 0;
    return result;
}

HOOK(0x00464930, AdvanceEntryCursor, "auto");
