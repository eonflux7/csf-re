#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Returns true when the dword at +0x8C is zero, unless the byte at +0x7C has bit 4 set.
static bool __fastcall Vehicle_IsSlotEmptyAndNotLocked(void* self, void* /*edx*/) {
    const bool slotEmpty = CSF_AT(int, self, 0x8c) == 0;
    if (slotEmpty && (CSF_AT(uint8_t, self, 0x7c) & 4) != 0) {
        return false;
    }
    return slotEmpty;
}

HOOK(0x41D160, Vehicle_IsSlotEmptyAndNotLocked, "auto");
