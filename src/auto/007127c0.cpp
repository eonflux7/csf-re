#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"
#include "core/memory.h"

// Returns the float stored at offset +0x88 of the object pointed to by self->field_0004.
static double __fastcall C_00863dd0_GetFloatAt88(C_00863dd0* self, void* /*edx*/)
{
    return CSF_AT(float, reinterpret_cast<uint8_t*>(self->field_0004), 0x88);
}

HOOK(0x007127C0, C_00863dd0_GetFloatAt88, "auto");
