#include <cstdint>
#include "globals.h"
#include "hook.h"

// __thiscall getter: returns the bound-context count stored at this+0xE4.
static unsigned int __fastcall SchedulerBase_GetNumberOfBoundContexts(void* self, void* /*edx*/)
{
    return CSF_AT(uint32_t, self, 0xE4);
}

HOOK(0x004FE490, SchedulerBase_GetNumberOfBoundContexts, "auto");
