#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Returns the 32-bit value stored at offset 0x1C0 of the object.
// Original: mov eax, [ecx+0x1C0]; ret   (no stack args)
static int __fastcall C_Unknown_GetField_01c0(void* self, void* /*edx*/)
{
    return CSF_AT(int, self, 0x1c0);
}

HOOK(0x492B30, C_Unknown_GetField_01c0, "auto");
