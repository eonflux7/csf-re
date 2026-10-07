#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// __thiscall method: stores the dword pointed to by the argument at this+0x14
// and returns the argument pointer unchanged.
static int* __fastcall SetField0014ReturnArg(void* self, void* /*edx*/, int* value)
{
    *CSF_AT(int*, self, 0x14) = *value;
    return value;
}

HOOK(0x420C80, SetField0014ReturnArg, "auto");
