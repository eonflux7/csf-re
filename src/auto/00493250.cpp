#include <cstdint>
#include "globals.h"

#include "hook.h"
#include "types/auto_classes.h"

// Resets the placement-related fields of a CPlayerCharacter to their
// "not yet chosen" state (-1, -1, 0) and returns -1.
static int __fastcall CPlayerCharacter_PreparePlacement(void* self, void* /*edx*/) {
    CSF_AT(int, self, 0x1b8) = -1;
    CSF_AT(int, self, 0x1bc) = -1;
    CSF_AT(int, self, 0x1c0) = 0;
    return -1;
}

HOOK(0x00493250, CPlayerCharacter_PreparePlacement, "auto");
