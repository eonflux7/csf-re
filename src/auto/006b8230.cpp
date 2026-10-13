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

// Reads the byte at offset 0x0D of the record referenced by the field at +0x10,
// or returns 2 when that reference is null.
static int __fastcall AttachmentRecord_Resolve(C_0085ec18* self, void* /*edx*/)
{
    uint32_t recordAddress = CSF_AT(uint32_t, self, 0x10);
    if (recordAddress != 0)
    {
        uint8_t* record = reinterpret_cast<uint8_t*>(recordAddress);
        return record[0x0D];
    }
    return 2;
}

HOOK(0x006B8230, AttachmentRecord_Resolve, "auto");
