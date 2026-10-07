#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// Returns whether the pointer-sized field at +0x218 of C_00865070 is non-zero.
static int __fastcall C_00865070_HasField0218(C_00865070* self, void* /*edx*/)
{
    return self->field_0218 != 0;
}

HOOK(0x00722C80, C_00865070_HasField0218, "auto");
