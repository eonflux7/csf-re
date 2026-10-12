#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GlobalStateValue_50DCF0, 0x0050DCF0, (void* self, void* edx));

// Looks up the entry-table record for animationSlot (16-byte records at self+0xC; +4 = begin, +8 = end
// of a uint32 list). If *indexInOut is out of range (or -1), picks a random index into the list using
// the global random state, stores it back in *indexInOut and returns the list value there; otherwise
// returns the list value at *indexInOut. Returns -1 when the list is empty.
static int __fastcall Tables_ResolveAnimationIndex(void* self, void* /*edx*/, int animationSlot, uint32_t* indexInOut)
{
    int result = -1;

    uint8_t* entry = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0xC)) + animationSlot * 16;
    uint32_t listBegin = CSF_AT(uint32_t, entry, 4);

    if (listBegin != 0 && ((CSF_AT(uint32_t, entry, 8) - listBegin) >> 2) != 0)
    {
        uint32_t listCount = (CSF_AT(uint32_t, entry, 8) - listBegin) >> 2;
        uint32_t requestedIndex = *indexInOut;

        if (requestedIndex == 0xFFFFFFFFu || requestedIndex >= listCount)
        {
            void* globalState = reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobalState_50D190()));
            uint32_t randomValue = static_cast<uint32_t>(GlobalStateValue_50DCF0(globalState, nullptr));
            uint32_t chosenIndex = randomValue % listCount;
            *indexInOut = chosenIndex;

            // The original reloads the table base and the list begin after the calls.
            uint8_t* entryAfter = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0xC)) + animationSlot * 16;
            uint8_t* beginAfter = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, entryAfter, 4));
            return *reinterpret_cast<int*>(beginAfter + chosenIndex * 4);
        }

        uint8_t* beginPtr = reinterpret_cast<uint8_t*>(listBegin);
        return *reinterpret_cast<int*>(beginPtr + requestedIndex * 4);
    }

    return result;
}

HOOK(0x00519B50, Tables_ResolveAnimationIndex, "auto");
