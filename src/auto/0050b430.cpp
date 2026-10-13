#include "globals.h"
#include "hook.h"

// Stores the detached flag byte at offset 0x11 of the _UnrealizedChore object.
// The original copies the low byte of the stack argument as-is, so the parameter is a raw byte.
// __thiscall with one stack argument: ret 4.
static void __fastcall UnrealizedChore_SetDetached(void* self, void* /*edx*/, uint8_t detached)
{
    CSF_AT(uint8_t, self, 0x11) = detached;
}

HOOK(0x0050B430, UnrealizedChore_SetDetached, "auto");
