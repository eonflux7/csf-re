#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original:  mov dword ptr [ecx+0x48], 0 ; ret
// __thiscall with no stack arguments (ret 0): ecx = self, edx unused.
// Stores the immediate 0 into the dword field at offset 0x48 of the object.
static void __fastcall C_Unknown_ClearField0048(void* self, void* /*edx*/)
{
    CSF_AT(uint32_t, self, 0x48) = 0;
}

HOOK(0x004E5270, C_Unknown_ClearField0048, "auto");
