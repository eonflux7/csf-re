#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Returns whether the 32-bit field at offset +0x8 is not 0xFFFFFFFF.
static int __fastcall HasAssignedId(void* self, void* /*edx*/)
{
    return CSF_AT(uint32_t, self, 0x8) != 0xFFFFFFFFu;
}

HOOK(0x4AD800, HasAssignedId, "auto");
