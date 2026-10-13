#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns the address of record `index` in the table whose base pointer is
// stored at +0x54. Records are 44 bytes; the first record starts 32 bytes in.
static int __fastcall C_008629a4_GetRecordAddress(C_008629a4* self, void* /*edx*/, int index) {
    uint32_t tableBase = CSF_AT(uint32_t, self, 0x54);
    uint32_t recordOffset = static_cast<uint32_t>(index) * 44u;
    return static_cast<int>(tableBase + recordOffset + 32u);
}

HOOK(0x708F80, C_008629a4_GetRecordAddress, "auto");
