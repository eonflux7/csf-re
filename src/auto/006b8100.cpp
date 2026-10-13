#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"

// Returns 1 when any bit of filterMask is set in the object's flags word at +0x04, otherwise 0.
static int __fastcall GfxCollision_AttachmentMatchesFilter(void* self, void* /*edx*/, int filterMask) {
    int attachmentFlags = CSF_AT(int, self, 0x04);
    return (filterMask & attachmentFlags) != 0 ? 1 : 0;
}

HOOK(0x6B8100, GfxCollision_AttachmentMatchesFilter, "auto");
