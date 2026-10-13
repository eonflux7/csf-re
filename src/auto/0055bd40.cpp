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

// Vtable pointer stored at 0x00845368 (referenced by address).
// Reads the flag at +0x4, stores the vtable pointer at +0x0, and clears the
// flag if it was set. Returns the previous (unmodified) flag value.
static char __fastcall C_00845368_ResetVtableAndClearFlag(C_00845368* self, void* /*edx*/)
{
    char oldFlag = static_cast<char>(self->field_0004);
    *reinterpret_cast<void**>(self) = reinterpret_cast<void*>(CSF_VTBL(0x845368));
    if (oldFlag)
        self->field_0004 = 0;
    return oldFlag;
}

HOOK(0x55BD40, C_00845368_ResetVtableAndClearFlag, "auto");
