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

// 0x6C9FD0 -- __thiscall(_BYTE* this, int a2), callee pops 4 bytes.
CSF_FUNC(void, __fastcall, C_00860a30_ClearField000c, 0x6C9FD0, (void* self, void* /*edx*/, int a2));

// 0x6DC9A0 -- tail jump to C_00860a30_ClearField000c: forwards (this, a2) unchanged.
static void __fastcall C_00861508_ClearField000c(C_00861508* self, void* /*edx*/, int a2)
{
    C_00860a30_ClearField000c(self, nullptr, a2);
}

HOOK(0x6DC9A0, C_00861508_ClearField000c, "auto");
