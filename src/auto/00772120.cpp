#include <cstdint>
#include "globals.h"
#include "hook.h"

// Returns the animation entry at `entryIndex` (96 bytes per entry) from the table
// referenced by the second pointer stored in this->field_4.
static int __fastcall SceneMissionSetup_GetAnimationEntry(void* self, void* /*edx*/, int entryIndex)
{
    // this->field_4 holds a pointer to a record; that record's +4 holds the table base.
    uint32_t recordAddr = CSF_AT(uint32_t, self, 0x4);
    int tableBase = CSF_AT(int, reinterpret_cast<void*>(recordAddr), 0x4);
    return tableBase + entryIndex * 96;
}

HOOK(0x00772120, SceneMissionSetup_GetAnimationEntry, "auto");
