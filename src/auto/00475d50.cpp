#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall C_008316d8_FireWithConfiguredParameter(
    C_008316d8* self, void* /*edx*/)
{
    return self->vSlot7C(
        CSF_AT(int, reinterpret_cast<void*>(self->field_0040), 0x130),
        1000002u, 1, 1);
}

HOOK(0x00475D50, C_008316d8_FireWithConfiguredParameter, "auto");
