#include <cstdint>
#include "globals.h"
#include "types/auto_classes.h"
#include "hook.h"

// Copies the four dwords stored at this+0x1C into the caller-supplied buffer
// and returns that buffer.
static uint32_t* __fastcall C_00860e68_CopyQuadAt001C(C_00860e68* self, void* /*edx*/, uint32_t* out)
{
    out[0] = CSF_AT(uint32_t, self, 0x1C);
    out[1] = CSF_AT(uint32_t, self, 0x20);
    out[2] = CSF_AT(uint32_t, self, 0x24);
    out[3] = CSF_AT(uint32_t, self, 0x28);
    return out;
}

HOOK(0x006D0D00, C_00860e68_CopyQuadAt001C, "auto");
