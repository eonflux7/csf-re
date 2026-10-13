#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Adds a float delta to the float stored at +0x70 of the object (C_008629f8).
static void __fastcall C_008629f8_AddToAccumulatedValue(C_008629f8* self, void* /*edx*/, float delta)
{
    self->field_0070 = delta + self->field_0070;
}

HOOK(0x6F8190, C_008629f8_AddToAccumulatedValue, "auto");
