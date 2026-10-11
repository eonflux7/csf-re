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

// Returns the address of the embedded cache-entry sub-object (at +0x20) of a
// C_0083e0d8 and marks that entry as valid (flag at +0x30 set, +0x28 cleared).
static int __fastcall GetCachedEntryObject(C_0083e0d8* self, void* /*edx*/)
{
    self->field_0030 = 1;
    self->field_0028 = 0;
    return (int)(uintptr_t)((uint8_t*)self + 0x20);
}

HOOK(0x0075C260, GetCachedEntryObject, "auto");
