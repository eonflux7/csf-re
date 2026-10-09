#include <cstdint>
#include "globals.h"
#include "hook.h"

// __thiscall getter: returns the default execution-context pointer stored at +0xEC (dword index 59).
static void* __fastcall UmsVirtualProcessor_GetDefaultDestination(void* self, void* /*edx*/)
{
    return CSF_AT(void*, self, 0xEC);
}

HOOK(0x004FE4A0, UmsVirtualProcessor_GetDefaultDestination, "auto");
