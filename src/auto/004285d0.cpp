#include <cstdint>
#include "globals.h"
#include "hook.h"

// Cheat-menu entry state check: each entry is 20 bytes; its state dword sits at +0x0C.
// Returns true when the entry's state equals 2.
static bool __fastcall MenuControls_IsEntrySetToTwo(void* self, void* /*edx*/, int entryIndex) {
    int entryState = CSF_AT(int, self, 0x0C + entryIndex * 20);
    return entryState == 2;
}

HOOK(0x4285D0, MenuControls_IsEntrySetToTwo, "auto");
