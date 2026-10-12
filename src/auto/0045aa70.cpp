#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, LookupEntryIndex, 0x004DC5F0, (void* self, void* edx, float* a2));
CSF_FUNC(int, __fastcall, ResolveEntryStateRecord, 0x004DC6E0, (void* self, void* edx, int a2));

// Returns true when the entry's state record matches the requested state filter.
// acceptStateOne selects state 1, acceptStateTwo selects state 2, and neither set selects state 0.
// If the entry is not found, returns true only when neither filter flag is set.
static bool __stdcall HudStatusIcons_EntryMatchesFilter(int entryId, char acceptStateOne, char acceptStateTwo)
{
    GetValue8B0554();
    uint8_t* owner = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(GetValue8B0554()));
    uint8_t* lookupObj = CSF_AT(uint8_t*, owner, 0x1FD8) + 0x34;

    int entryIndex = LookupEntryIndex(lookupObj, nullptr, reinterpret_cast<float*>(static_cast<uintptr_t>(entryId)));
    if (entryIndex != -1)
    {
        int* stateRecord = reinterpret_cast<int*>(static_cast<uintptr_t>(ResolveEntryStateRecord(lookupObj, nullptr, entryIndex)));
        if ((acceptStateOne && *stateRecord == 1)
            || (acceptStateTwo && *stateRecord == 2)
            || (!acceptStateOne && !acceptStateTwo && *stateRecord == 0))
        {
            return true;
        }
        return false;
    }

    if (acceptStateOne)
        return false;
    if (acceptStateTwo)
        return false;
    return true;
}

HOOK(0x0045AA70, HudStatusIcons_EntryMatchesFilter, "auto");
