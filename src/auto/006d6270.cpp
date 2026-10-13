#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns the active virtual processor count stored at +0xF8 of the scheduler object.
static unsigned int __fastcall C_00861120_GetNumberOfActiveVirtualProcessors(C_00861120* self, void* /*edx*/) {
    return self->field_00f8;
}

HOOK(0x006D6270, C_00861120_GetNumberOfActiveVirtualProcessors, "auto");
