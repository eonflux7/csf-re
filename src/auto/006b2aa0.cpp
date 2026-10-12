#include <cstdint>
#include "globals.h"
#include "types/auto_classes.h"
#include "hook.h"

// Copies the four dwords stored at this+0x8C into the caller-supplied buffer
// and returns that buffer.
static uint32_t* __fastcall C_00860288_CopyQuadAt008C(C_00860288* self, void* /*edx*/, uint32_t* out)
{
    out[0] = CSF_AT(uint32_t, self, 0x8C);
    out[1] = CSF_AT(uint32_t, self, 0x90);
    out[2] = CSF_AT(uint32_t, self, 0x94);
    out[3] = CSF_AT(uint32_t, self, 0x98);
    return out;
}

HOOK(0x006B2AA0, C_00860288_CopyQuadAt008C, "auto");
