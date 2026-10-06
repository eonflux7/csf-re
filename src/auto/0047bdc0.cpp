#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns the localized name of the crouch mode.
// The original just loads the address of the "TModoAgacharse" string constant
// into EAX and returns it (ret 0, no stack arguments, `this` unused).
static const char* __fastcall C_00832bf8_GetCrouchModeName(C_00832bf8* self, void* /*edx*/)
{
    (void)self;
    return CSF_STR(0x00832C90, "TModoAgacharse");
}

HOOK(0x47BDC0, C_00832bf8_GetCrouchModeName, "auto");
