#include "types/auto_classes.h"
#include "hook.h"

// __thiscall getter: returns the dword stored at offset 0xBC of the object.
static int __fastcall C_008670f0_GetField00bc(C_008670f0* self, void* /*edx*/)
{
    return static_cast<int>(self->field_00bc);
}

HOOK(0x007349F0, C_008670f0_GetField00bc, "auto");
