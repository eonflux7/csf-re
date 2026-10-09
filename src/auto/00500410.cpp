#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns the address of a per-entry field inside the scene-entry array at this+0x30.
// Entries are 88 (0x58) bytes; the field offset depends on the state stored at [stateOwner+0x54]:
// state 2 -> +48, state 3 -> +60, anything else (including 1) -> +36.
static int __fastcall CSceneLoader_GetEntryFieldAddress(C_0083f5d8* self, void* /*edx*/, int entryIndex, int stateOwner)
{
    uint8_t* owner = reinterpret_cast<uint8_t*>(stateOwner);
    int state = CSF_AT(int, owner, 0x54);

    int fieldOffset;
    if (state == 1) {
        fieldOffset = 0x24;
    } else if (state == 2) {
        fieldOffset = 0x30;
    } else if (state == 3) {
        fieldOffset = 0x3C;
    } else {
        fieldOffset = 0x24;
    }

    int entryArrayBase = CSF_AT(int, self, 0x30);
    return entryArrayBase + entryIndex * 0x58 + fieldOffset;
}

HOOK(0x500410, CSceneLoader_GetEntryFieldAddress, "auto");
