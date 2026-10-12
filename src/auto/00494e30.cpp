#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Clears three dword fields (+0x2E0, +0x2E4, +0x2E8) of the object and returns 0.
// __thiscall with no stack arguments: callee pops 0 bytes.
static int __fastcall HudAlertIcons_ResetOrientationFields(void* self, void* /*edx*/) {
    CSF_AT(int, self, 0x2E0) = 0;
    CSF_AT(int, self, 0x2E4) = 0;
    CSF_AT(int, self, 0x2E8) = 0;
    return 0;
}

HOOK(0x00494E30, HudAlertIcons_ResetOrientationFields, "auto");
