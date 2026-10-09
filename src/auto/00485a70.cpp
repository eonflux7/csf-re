#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Walks the entry array held in field_0018..field_001c and counts the entries
// whose virtual slot 0x98 check returns true.
static int __fastcall CCharActionFsm_CountQualifyingEntries(CCharActionFsm* self, void* /*edx*/) {
    int matchCount = 0;
    uint32_t index = 0;
    for (;;) {
        // The original reloads the array begin and end on every iteration.
        uint32_t entriesBegin = self->field_0018;
        if (entriesBegin == 0) {
            break;
        }
        int entryCount = (int)(self->field_001c - entriesBegin) >> 2;
        if (index >= (uint32_t)entryCount) {
            break;
        }
        uint32_t* entryTable = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(entriesBegin));
        void* entry = reinterpret_cast<void*>(static_cast<uintptr_t>(entryTable[index]));
        // Entry type is unknown here: dispatch through its vtable, slot 0x98 / 4.
        auto entryCheck = reinterpret_cast<uint8_t(__fastcall*)(void*, void*)>(
            CSF_AT(void**, entry, 0)[0x98 / 4]);
        if (entryCheck(entry, nullptr)) {
            ++matchCount;
        }
        ++index;
    }
    return matchCount;
}

HOOK(0x00485A70, CCharActionFsm_CountQualifyingEntries, "auto");
