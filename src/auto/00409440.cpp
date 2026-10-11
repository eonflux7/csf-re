#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Resets the target controller state of a CharActionFsm: sets the value at
// +0x20 to 1.0f and clears the three dwords at +0x58, +0x5C and +0x60.
static int __fastcall CharActionFsm_ResetTargetController(C_00863430* self, void* /*edx*/)
{
    CSF_AT(float, self, 0x20) = 1.0f;
    CSF_AT(int, self, 0x58) = 0;
    CSF_AT(int, self, 0x5C) = 0;
    CSF_AT(int, self, 0x60) = 0;
    return 0;
}

HOOK(0x00409440, CharActionFsm_ResetTargetController, "auto");
