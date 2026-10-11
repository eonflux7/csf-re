#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// 0x006E8120: __cdecl(int a1, int a2), caller pops 8 bytes.
CSF_FUNC(int, __cdecl, C_00861e98_QueryInnerValue, 0x006E8120, (int innerArg, int a2));

// __thiscall int C_00861e98::QueryInnerOrMinusOne(int a2) -> ret 4
// Returns -1 when the "has inner" flag at +0x1c is zero; otherwise forwards
// the inner object pointer at +0x14 and a2 to the inner query function.
static int __fastcall C_00861e98_QueryInnerOrMinusOne(C_00861e98* self, void* /*edx*/, int a2)
{
    int result = -1;
    if (CSF_AT(int, self, 0x1c) != 0)
    {
        return C_00861e98_QueryInnerValue(CSF_AT(int, self, 0x14), a2);
    }
    return result;
}

HOOK(0x006E89E0, C_00861e98_QueryInnerOrMinusOne, "auto");
