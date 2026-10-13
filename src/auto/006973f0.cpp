#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"

// Returns true when the archive has no entries: the begin pointer at +4 is null,
// or the element count (end - begin) / 32 at +8 is zero.
static bool __fastcall CoreFiles_IsArchiveAccessLocked(void* self, void* /*edx*/)
{
    int32_t beginPtr = CSF_AT(int32_t, self, 0x4);
    if (beginPtr == 0) {
        return true;
    }
    int32_t endPtr = CSF_AT(int32_t, self, 0x8);
    int32_t entryCount = (endPtr - beginPtr) >> 5;
    return entryCount == 0;
}

HOOK(0x006973F0, CoreFiles_IsArchiveAccessLocked, "auto");
