#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, MenuMessageBox_ResolveEntryByKey, 0x00692440, (int code, int flag));
CSF_FUNC(void, __cdecl, MenuLoading_CopyLocalizedText, 0x006956C0, (uint16_t* destination, const uint16_t* source, int capacity));

// Writes the localized text for rank 1..5 into destination (capacity = a3).
// Returns 1 when a rank key was handled, 0 otherwise.
static char MenuMissionInfo_FormatRank(int rankIndex, int destinationAddress, int capacity)
{
    int textKey = 0;
    char result = 0;

    switch (rankIndex)
    {
    case 1:
        textKey = 1731213620;
        break;
    case 2:
        textKey = 1731213621;
        break;
    case 3:
        textKey = 1731213622;
        break;
    case 4:
        textKey = 1731213623;
        break;
    case 5:
        textKey = 1731213624;
        break;
    default:
        return 0;
    }

    int resolvedEntry = MenuMessageBox_ResolveEntryByKey(textKey, 8);
    MenuLoading_CopyLocalizedText(reinterpret_cast<uint16_t*>(destinationAddress),
                                  reinterpret_cast<const uint16_t*>(resolvedEntry),
                                  capacity);
    result = 1;
    return result;
}

HOOK(0x007614B0, MenuMissionInfo_FormatRank, "auto");
