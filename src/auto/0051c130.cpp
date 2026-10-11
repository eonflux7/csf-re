#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// DNameNode constructor (C_00840ea4): installs C_00840ea4's vtable and zeroes the field at +0x04.
// __thiscall (ecx = this), ret 0, returns this in eax.
static C_00840ea4* __fastcall C_00840ea4_Construct(C_00840ea4* self, void* /*edx*/) {
    *(void***)self = CSF_VTBL(0x00840EA4);
    CSF_AT(int, self, 0x04) = 0;
    return self;
}

HOOK(0x0051C130, C_00840ea4_Construct, "auto");
