#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Returns the int stored at offset 0x28 of the 0x2C-byte entry at index `index`
// in the array whose base pointer lives at self+0x54.
static int __fastcall C_008629a4_GetEntryValueAt(void* self, void* /*edx*/, int index)
{
    uint8_t* entries = CSF_AT(uint8_t*, self, 0x54);
    return CSF_AT(int, entries + index * 44, 0x28);
}

HOOK(0x708F60, C_008629a4_GetEntryValueAt, "auto");
