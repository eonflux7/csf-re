#include <cstdint>
#include "globals.h"
#include "hook.h"

// sub_41E3D0: returns the dword field at offset 0x14 of the object plus 0x5C.
// __thiscall, no stack arguments (ret 0).
static int __fastcall C_Unk41E3D0_GetField14Plus5C(void* self, void* /*edx*/)
{
    return static_cast<int>(CSF_AT(uint32_t, self, 0x14)) + 0x5C;
}

HOOK(0x41E3D0, C_Unk41E3D0_GetField14Plus5C, "auto");
