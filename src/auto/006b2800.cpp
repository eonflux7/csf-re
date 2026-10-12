#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Compares the dword at *self with the dword at *other (stack argument).
// Returns 1 when they are equal, 0 otherwise.
static int __fastcall HudCheckFlagAgainstGlobal(uint32_t* self, void* /*edx*/, uint32_t* other) {
    return (*other == *self) ? 1 : 0;
}

HOOK(0x006B2800, HudCheckFlagAgainstGlobal, "auto");
