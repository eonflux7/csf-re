#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// 0x0073F760: stores 0 into the second dword-sized word of the object
// (offset +0x04). No other reads/writes, no side effects.
static void __fastcall ClearDwordField4(void* self, void* /*edx*/)
{
    CSF_AT(uint32_t, self, 0x4) = 0;
}

HOOK(0x73F760, ClearDwordField4, "auto");
