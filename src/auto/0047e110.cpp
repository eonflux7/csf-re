#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// 0x0047E110
// Returns the signed number of 12-byte entries between the begin pointer at
// +0x0C and the end pointer at +0x10, or 0 when the begin pointer is null.
static int __fastcall C_00832eec_GetRecordCount(C_00832eec* self, void* /*edx*/)
{
    const int32_t begin = static_cast<int32_t>(self->field_000c);
    if (begin == 0)
        return 0;

    const int32_t end = static_cast<int32_t>(self->field_0010);
    return (end - begin) / 12;
}

HOOK(0x0047E110, C_00832eec_GetRecordCount, "auto");
