#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GlobalStateValue_50DCF0, 0x0050DCF0, (void* self, void* edx));

// Returns the entry at the given index of the table's per-slot lists, selected by the
// global state value modulo the list length; -1 if the list is empty.
static int __fastcall SceneSubsystem_LookupEntry(void* self, void* /*edx*/, int entryIndex)
{
    uint32_t entryOffset = static_cast<uint32_t>(entryIndex) << 4;
    uint8_t* table = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x0C));
    uint8_t* entry = table + entryOffset;

    uint32_t itemsBegin = CSF_AT(uint32_t, entry, 4);
    int result = -1;
    if (itemsBegin != 0)
    {
        int itemCount = static_cast<int>(CSF_AT(uint32_t, entry, 8) - itemsBegin) >> 2;
        if (itemCount != 0)
        {
            uint32_t divisor = static_cast<uint32_t>(itemCount);
            int globalState = GetGlobalState_50D190();
            uint32_t stateValue = static_cast<uint32_t>(GlobalStateValue_50DCF0(
                reinterpret_cast<void*>(static_cast<uintptr_t>(globalState)), nullptr));
            uint32_t slot = stateValue % divisor;

            // Re-read the table and list pointer after the calls, as the original does.
            uint8_t* tableAgain = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x0C));
            uint8_t* entryAgain = tableAgain + entryOffset;
            uint32_t* items = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, entryAgain, 4));
            result = static_cast<int>(items[slot]);
        }
    }
    return result;
}

HOOK(0x00519AF0, SceneSubsystem_LookupEntry, "auto");
