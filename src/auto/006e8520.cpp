#include "types/auto_classes.h"
#include "hook.h"

// Returns bit 2 of field_0058 (0 or 1).
static int __fastcall C_00861e98_IsBit2OfField58Set(C_00861e98* self, void* /*edx*/)
{
    return (self->field_0058 >> 2) & 1;
}

HOOK(0x006E8520, C_00861e98_IsBit2OfField58Set, "auto");
