#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// Copies the four dwords stored at this+0x34 into the caller-supplied buffer
// and returns that buffer.
static uint32_t* __fastcall C_00865870_CopyQuadAt0034(C_00865870* self, void* /*edx*/, uint32_t* out)
{
    out[0] = self->field_0034;
    out[1] = self->field_0038;
    out[2] = self->field_003c;
    out[3] = self->field_0040;
    return out;
}

HOOK(0x00728610, C_00865870_CopyQuadAt0034, "auto");
