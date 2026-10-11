#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Returns true when the 20-byte cheat-table entry at `index` has a zero
// dword at +0x0C (the original does: eax = index*5; test [self + eax*4 + 0xC], 0; sete).
static bool __fastcall IsCheatEntryInactive(void* self, void* /*edx*/, int index) {
    uint8_t* entryBase = reinterpret_cast<uint8_t*>(self) + index * 20;
    int entryFlag = CSF_AT(int, entryBase, 0xC);
    return entryFlag == 0;
}

HOOK(0x428590, IsCheatEntryInactive, "auto");
