#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns the address of an embedded sub-object at +0x2B8 inside the object
// pointed to by field_0308 (a plain byte offset computed off that base pointer).
static uint32_t __fastcall C_0083dbb0_GetBasePtr0308Member2B8(C_0083dbb0* self, void* /*edx*/)
{
    return self->field_0308 + 0x2B8;
}

HOOK(0x4D7790, C_0083dbb0_GetBasePtr0308Member2B8, "auto");
