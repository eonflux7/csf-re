#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Original callee: sub_6C04E0(int a1), __cdecl, pops 0 bytes.
CSF_FUNC(int, __cdecl, Water_ProcessHandle, 0x006C04E0, (int a1));

// Calls Water_ProcessHandle on the handle stored at +0x08 unless the
// re-entrancy flag at +0xCC is already set. The flag is raised for the
// duration of the call and cleared afterwards. Returns the handle value.
static int __fastcall Water_GuardedProcess(void* self, void* /*edx*/, int /*a2*/)
{
    int result = CSF_AT(int, self, 0x8);
    if (result != 0)
    {
        if (CSF_AT(uint8_t, self, 0xCC) == 0)
        {
            CSF_AT(uint8_t, self, 0xCC) = 1;
            result = Water_ProcessHandle(result);
            CSF_AT(uint8_t, self, 0xCC) = 0;
        }
    }
    return result;
}

HOOK(0x0070CD10, Water_GuardedProcess, "auto");
