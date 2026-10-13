#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns the address of entry `index` in the table whose base pointer is
// stored at +0x54. Entries are 44 bytes; the first entry starts 4 bytes in.
static int __fastcall C_008629a4_GetEntryAddressAtIndex(C_008629a4* self, void* /*edx*/, int index) {
    uint32_t tableBase = CSF_AT(uint32_t, self, 0x54);
    uint32_t entryOffset = static_cast<uint32_t>(index) * 44u;
    return static_cast<int>(tableBase + entryOffset + 4u);
}

HOOK(0x708FA0, C_008629a4_GetEntryAddressAtIndex, "auto");
