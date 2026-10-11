#include <cstdint>
#include "globals.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "hook.h"

// 0x004A31E0
// Returns the signed number of 12-byte entries held between the
// begin pointer at +0x04 and the end pointer at +0x08, or 0 when
// the begin pointer is null.
static int __fastcall S_004a31e0_GetTwelveByteEntryCount(S_004a31e0* self, void* /*edx*/)
{
    const int32_t begin = static_cast<int32_t>(self->field_0004);
    if (begin == 0)
        return 0;

    const int32_t end = static_cast<int32_t>(self->field_0008);
    return (end - begin) / 12;
}

HOOK(0x004A31E0, S_004a31e0_GetTwelveByteEntryCount, "auto");
