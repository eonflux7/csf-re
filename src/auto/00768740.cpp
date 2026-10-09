#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Returns the address of entry `slotIndex` in the 24-byte-stride slot table
// whose base pointer is stored at +0x08 of this object.
static int __fastcall C_0085e7b8_GetSlotEntry(C_0085e7b8* self, void* /*edx*/, int slotIndex) {
    return static_cast<int>(self->field_0008 + static_cast<uint32_t>(slotIndex) * 24u);
}

HOOK(0x00768740, C_0085e7b8_GetSlotEntry, "auto");
