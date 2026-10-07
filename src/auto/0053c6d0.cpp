#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Zeroes the 32-bit member at offset 0x24 of the object.
static void __fastcall CMenuSoundOptions_ClearField0024(CMenuSoundOptions* self, void* /*edx*/)
{
    CSF_AT(uint32_t, self, 0x24) = 0;
}

HOOK(0x0053C6D0, CMenuSoundOptions_ClearField0024, "auto");
