#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"

static int __fastcall InterfaceSetMenuState(void* self, void* /*edx*/, int a2)
{
    CSF_AT(int, self, 0xC) = 0;
    CSF_AT(int, self, 0x8) = a2;
    return a2;
}

HOOK(0x0050DC10, InterfaceSetMenuState, "auto");
