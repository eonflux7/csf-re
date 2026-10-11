#include <cstdint>

#include "hook.h"

// Walks a table of 93 entries (20-byte stride, first entry at +0x0C) and turns
// state 1 or 2 into state 3. Returns the value read on the final iteration,
// before any update.
static int __fastcall Cheats_NormalizeEntryStates(int* self, void* /*edx*/) {
    int* entry = self + 3;
    int remainingEntries = 93;
    int lastValue = 0;
    do {
        lastValue = *entry;
        if (*entry == 1 || lastValue == 2) {
            *entry = 3;
        }
        entry += 5;
        --remainingEntries;
    } while (remainingEntries != 0);
    return lastValue;
}

HOOK(0x00428820, Cheats_NormalizeEntryStates, "auto");
