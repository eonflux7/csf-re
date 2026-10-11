#include <cstdint>
#include "globals.h"
#include <cstddef>

#include "hook.h"
#include "types/auto_classes.h"

// Looks up a dword in a two-level table hanging off `this`.
// The first level is an array of 16-byte entries starting at +0x24; each entry
// holds a pointer to a row of dwords. Returns row[a3].
static int __fastcall C_00860bb8_LookupDword(C_00860bb8* self, void* /*edx*/, int a2, int a3) {
    uint32_t rowAddress = CSF_AT(uint32_t, self, 0x24 + a2 * 16);
    int* row = reinterpret_cast<int*>(rowAddress);
    return row[a3];
}

HOOK(0x6CB890, C_00860bb8_LookupDword, "auto");
