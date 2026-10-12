#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// int __thiscall (C_0082de40* this, int index): clears the active byte (+12) of
// entry `index` in the 24-byte record table at this->field_0010 and returns index * 3.
static int __fastcall C_0082de40_ClearEntryActive(C_0082de40* self, void* /*edx*/, int index) {
    uint8_t* entryTable = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(self->field_0010));
    CSF_AT(uint8_t, entryTable, 24u * static_cast<uint32_t>(index) + 0xCu) = 0;
    return 3 * index;
}

HOOK(0x40F7D0, C_0082de40_ClearEntryActive, "auto");
