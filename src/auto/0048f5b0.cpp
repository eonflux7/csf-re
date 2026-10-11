#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static char __fastcall S_0048f5b0_EnableObjectAndLinkedFlag(
    S_0048f5b0* self, void* /*edx*/)
{
    self->field_0171 = 1;
    void* linkedObject = reinterpret_cast<void*>(self->field_00a8);
    CSF_AT(uint8_t, linkedObject, 0x1C) = 1;
    return 1;
}

HOOK(0x0048F5B0, S_0048f5b0_EnableObjectAndLinkedFlag, "auto");
