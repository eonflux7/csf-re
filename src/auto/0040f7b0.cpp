#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// int __thiscall (C_0082de40* this, int index): marks entry `index` active
// (byte at +12 of its 24-byte record) and returns index * 3.
static int __fastcall C_0082de40_MarkEntryActive(C_0082de40* self, void* /*edx*/, int index) {
    uint8_t* entryTable = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(self->field_0010));
    CSF_AT(uint8_t, entryTable, 24u * static_cast<uint32_t>(index) + 0xCu) = 1;
    return 3 * index;
}

HOOK(0x40F7B0, C_0082de40_MarkEntryActive, "auto");
