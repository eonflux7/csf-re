#include <cstdint>
#include "globals.h"
#include "types/auto_classes.h"
#include "hook.h"

// Returns true when the dword at offset 0x10 of this object is zero.
static int __fastcall IsField10Null(void* self, void* /*edx*/)
{
    return CSF_AT(uint32_t, self, 0x10) == 0;
}

HOOK(0x4DB5B0, IsField10Null, "auto");
