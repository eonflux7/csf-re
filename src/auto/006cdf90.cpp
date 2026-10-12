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

// Returns the int stored at offset 0x60 of the record at index `index` in the
// array whose base pointer lives at self+0x08. Records are 0x64 bytes each.
static int __fastcall GfxCollisionShapes_GetRecordValue(void* self, void* /*edx*/, int index)
{
    uint8_t* records = CSF_AT(uint8_t*, self, 0x08);
    return CSF_AT(int, records + index * 100, 0x60);
}

HOOK(0x6CDF90, GfxCollisionShapes_GetRecordValue, "auto");
