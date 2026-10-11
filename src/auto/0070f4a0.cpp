#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Callees, bound by original address.
CSF_FUNC(int, __cdecl, sub_7DBE90, 0x007DBE90, ());
CSF_FUNC(int, __cdecl, RtDictGetNumEntries, 0x007DBD80, (int a1));
CSF_FUNC(int, __cdecl, RtDictGetEntry, 0x007DBD70, (int a1, int a2));
CSF_FUNC(int*, __cdecl, _rtDictRemoveEntryFromAllDictionaries, 0x007DBE40, (int a1));
CSF_FUNC(int, __cdecl, sub_7DBC20, 0x007DBC20, (int a1));
CSF_FUNC(uint32_t*, __cdecl, sub_7DBBA0, 0x007DBBA0, (int a1));
CSF_FUNC(int, __cdecl, RpMTEffectDictSetCurrent, 0x007DBE80, (int a1, int a2));

// Empties every effect dictionary returned by sub_7DBE90, releases that
// dictionary, then makes the default dictionary (sub_7DBBA0(0)) current.
// Returns the result of RpMTEffectDictSetCurrent.
static int __cdecl AudioEffectDicts_ClearAndResetCurrent() {
    const int dict = sub_7DBE90();
    if (dict) {
        int numEntries = RtDictGetNumEntries(dict);
        while (numEntries > 0) {
            --numEntries;
            const int entry = RtDictGetEntry(dict, numEntries);
            if (entry) {
                _rtDictRemoveEntryFromAllDictionaries(entry);
            }
        }
        sub_7DBC20(dict);
    }
    const int defaultDict =
        static_cast<int>(reinterpret_cast<uintptr_t>(sub_7DBBA0(0)));
    return RpMTEffectDictSetCurrent(defaultDict, 0);
}

HOOK(0x0070F4A0, AudioEffectDicts_ClearAndResetCurrent, "auto");
