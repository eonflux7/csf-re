#include <cstdint>
#include "globals.h"
#include "hook.h"

// Cheat-menu entry state check: each entry is 20 bytes; its state dword sits at +0x0C.
// Returns true when the entry's state equals 3.
static bool __fastcall CheatsMenu_IsEntryStateThree(void* self, void* /*edx*/, int entryIndex) {
    int entryState = CSF_AT(int, self, 0x0C + entryIndex * 20);
    return entryState == 3;
}

HOOK(0x4285F0, CheatsMenu_IsEntryStateThree, "auto");
