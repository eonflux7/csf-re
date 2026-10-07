#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Returns bit 5 of the dword at offset +0x4 of the object (unsigned shift).
static int __fastcall IsBit5Set(void* self, void* /*edx*/)
{
    return (CSF_AT(uint32_t, self, 0x4) >> 5) & 1;
}

HOOK(0x6B8190, IsBit5Set, "auto");
