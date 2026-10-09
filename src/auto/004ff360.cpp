#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Resets the scene loader's state block to its defaults. The object's
// class is not identified yet, so it is handled as a raw pointer.
// Field layout (offsets from the original stores):
//   +0x04 byte  flag, cleared
//   +0x14 int   cleared
//   +0x18 int   15
//   +0x1C byte  cleared
//   +0x1D byte  cleared
//   +0x20 int   cleared
//   +0x24 vtbl  C_0082d448 (sub-object)
//   +0x28 int   -1
//   +0x2C int   -1
//   +0x30 vtbl  C_0082d448 (sub-object)
//   +0x34 int   -1
//   +0x38 int   -1
//   +0x3C vtbl  C_0082d448 (sub-object)
//   +0x40 int   -1
//   +0x44 int   -1
//   +0x48 int   0
//   +0x4C int   0
//   +0x50 int   0
//   +0x54 int   10
// Original is __thiscall with no stack args (ret 0); returns this in eax.
static int __fastcall SceneLoader_ResetState(void* self, void* /*edx*/)
{
    CSF_AT(int, self, 0x18) = 15;
    CSF_AT(int, self, 0x14) = 0;
    CSF_AT(uint8_t, self, 0x04) = 0;

    CSF_AT(void*, self, 0x24) = CSF_VTBL(0x0082D448);
    CSF_AT(int, self, 0x28) = -1;
    CSF_AT(int, self, 0x2C) = -1;
    CSF_AT(void*, self, 0x30) = CSF_VTBL(0x0082D448);
    CSF_AT(int, self, 0x34) = -1;
    CSF_AT(int, self, 0x38) = -1;
    CSF_AT(void*, self, 0x3C) = CSF_VTBL(0x0082D448);
    CSF_AT(int, self, 0x40) = -1;
    CSF_AT(int, self, 0x44) = -1;

    CSF_AT(int, self, 0x48) = 0;
    CSF_AT(int, self, 0x4C) = 0;
    CSF_AT(int, self, 0x50) = 0;
    CSF_AT(int, self, 0x54) = 10;

    CSF_AT(uint8_t, self, 0x1C) = 0;
    CSF_AT(uint8_t, self, 0x1D) = 0;
    CSF_AT(int, self, 0x20) = 0;

    return reinterpret_cast<int>(self);
}

HOOK(0x4FF360, SceneLoader_ResetState, "auto");
