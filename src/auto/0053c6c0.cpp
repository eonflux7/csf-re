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

// 0x0053C6C0: clears the dword at offset 0x20 of the object (single store).
static void __fastcall CMenuSoundOptions_ClearField0020(CMenuSoundOptions* self, void* /*edx*/)
{
    CSF_AT(uint32_t, self, 0x20) = 0;
}

HOOK(0x0053C6C0, CMenuSoundOptions_ClearField0020, "auto");
