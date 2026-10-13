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

// Returns bit 8 of the flags word at +0x004 as 0 or 1.
static int __fastcall GetAttachmentRegionOverrideFlag(C_0085ec18* self, void* /*edx*/)
{
    return (self->field_0004 >> 8) & 1;
}

HOOK(0x006B8150, GetAttachmentRegionOverrideFlag, "auto");
