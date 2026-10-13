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

// Returns bit 7 of the attachment flags word (+0x004) as a 0/1 int.
static int __fastcall ObjectAttachment_HasSoundTrigger(C_0085ec18* self, void* /*edx*/)
{
    return (self->field_0004 >> 7) & 1;
}

HOOK(0x006B8140, ObjectAttachment_HasSoundTrigger, "auto");
