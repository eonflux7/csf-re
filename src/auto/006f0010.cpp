#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns the dword field at offset 0x94 of the object (this).
static int __fastcall C_Unknown_GetField0094(void* self, void* /*edx*/)
{
    return CSF_AT(int, self, 0x94);
}

HOOK(0x006F0010, C_Unknown_GetField0094, "auto");
