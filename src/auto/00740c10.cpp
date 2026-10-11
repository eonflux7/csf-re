#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Zeroes the 16-bit field at byte offset `offset` of the object if it is nonzero.
static void MpSpawnPoints_ClearWordAtOffsetIfSet(void* self, uint32_t offset) {
    if (CSF_AT(uint16_t, self, offset) != 0) {
        CSF_AT(uint16_t, self, offset) = 0;
    }
}

// Clears six 16-bit flag fields of the spawn-point object, in the original's order.
// Returns 0 (the original's result variable is never modified).
static int __fastcall MpSpawnPoints_NotifyRelatedOwner(void* self, void* /*edx*/) {
    int result = 0;

    MpSpawnPoints_ClearWordAtOffsetIfSet(self, 0x68C);  // index 0 (word 838)
    MpSpawnPoints_ClearWordAtOffsetIfSet(self, 0x680);  // index 1 (word 832)
    MpSpawnPoints_ClearWordAtOffsetIfSet(self, 0x698);  // index 2 (word 844)
    MpSpawnPoints_ClearWordAtOffsetIfSet(self, 0x692);  // index 3 (word 841)
    MpSpawnPoints_ClearWordAtOffsetIfSet(self, 0x69E);  // index 4 (word 847)
    MpSpawnPoints_ClearWordAtOffsetIfSet(self, 0x686);  // index 5 (word 835)

    return result;
}

HOOK(0x740C10, MpSpawnPoints_NotifyRelatedOwner, "auto");
