#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Base-class cleanup for C_00872abc. Called as a tail jump from C_00840dac's destructor path.
CSF_FUNC(void, __fastcall, Tables_ReleaseBaseState, 0x0078C8E0, (void* self, void* edx));

// Sets the object's vtable to C_00840dac, then tail-jumps into the base cleanup.
// The tail jump forwards ecx (this) unchanged, so the callee receives the same object.
static int __fastcall C_00840dac_Cleanup(C_00840dac* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x00840DAC);
    Tables_ReleaseBaseState(self, nullptr);
    return 0;
}

HOOK(0x0051B4E0, C_00840dac_Cleanup, "auto");
