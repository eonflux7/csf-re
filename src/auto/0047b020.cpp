#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Resets the default state of an attack-task object: clears the dword at +0x010
// and marks the two sentinel slots at +0x01C and +0x020 as -1. Returns -1.
static int __fastcall C_00831ac0_ResetTaskDefaults(C_00831ac0* self, void* /*edx*/)
{
    CSF_AT(int, self, 0x10) = 0;
    CSF_AT(int, self, 0x1C) = -1;
    CSF_AT(int, self, 0x20) = -1;
    return -1;
}

HOOK(0x47B020, C_00831ac0_ResetTaskDefaults, "auto");
