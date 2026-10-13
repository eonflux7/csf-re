#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GlobalStateValue_50DCF0, 0x0050DCF0, (void* self, void* edx));

// Resolves the list entry at slot `animationSlot`: returns the element at the stored index,
// or picks a random index (writing it back) when the stored index is -1 or out of range.
// Returns -1 when the list is empty.
static int __fastcall Tables_ResolveAnimationIndex(void* self, void* /*edx*/, int animationSlot, uint32_t* storedIndex)
{
    uint8_t* slotEntry = CSF_AT(uint8_t*, self, 0x0C) + animationSlot * 16;
    uint32_t listBegin = CSF_AT(uint32_t, slotEntry, 0x04);
    int32_t listCount = static_cast<int32_t>(CSF_AT(uint32_t, slotEntry, 0x08) - listBegin) >> 2;

    if (listBegin == 0 || listCount == 0)
        return -1;

    uint32_t index = *storedIndex;
    if (index == 0xFFFFFFFFu || index >= static_cast<uint32_t>(listCount))
    {
        uint32_t entryCount = 0;
        uint32_t currentBegin = CSF_AT(uint32_t, slotEntry, 0x04);
        if (currentBegin != 0)
            entryCount = static_cast<uint32_t>(static_cast<int32_t>(CSF_AT(uint32_t, slotEntry, 0x08) - currentBegin) >> 2);

        int globalState = GetGlobalState_50D190();
        uint32_t randomValue = static_cast<uint32_t>(
            GlobalStateValue_50DCF0(reinterpret_cast<void*>(static_cast<uintptr_t>(globalState)), nullptr));
        index = randomValue % entryCount;
        *storedIndex = index;

        uint8_t* entryNow = CSF_AT(uint8_t*, self, 0x0C) + animationSlot * 16;
        uint32_t beginNow = CSF_AT(uint32_t, entryNow, 0x04);
        return CSF_AT(int, reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(beginNow)), index * 4);
    }

    return CSF_AT(int, reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(listBegin)), index * 4);
}

HOOK(0x00519B50, Tables_ResolveAnimationIndex, "auto");
