#include <cstdint>
#include "globals.h"
#include "core/memory.h"
#include "hook.h"

// 0x004FE4C0
// Returns the signed number of 88-byte entries between the array begin
// pointer at +0x04 and the array end pointer at +0x08, or 0 when the
// begin pointer is null. The original computes the division by 88 with
// the signed magic-number multiply.
static int __fastcall SceneLoader_GetEntryCount88(void* self, void* /*edx*/)
{
    const int32_t begin = CSF_AT(int32_t, self, 0x04);
    if (begin == 0)
        return 0;

    const int32_t end = CSF_AT(int32_t, self, 0x08);
    return (end - begin) / 88;
}

HOOK(0x004FE4C0, SceneLoader_GetEntryCount88, "auto");
