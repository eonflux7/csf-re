#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall C_008316d8_FireWithAlternateConfiguredParameter(
    C_008316d8* self, void* /*edx*/)
{
    return self->vSlot7C(
        CSF_AT(int, reinterpret_cast<void*>(self->field_0040), 0x13C),
        1000001u, 1, 1);
}

HOOK(0x00475D70, C_008316d8_FireWithAlternateConfiguredParameter, "auto");
