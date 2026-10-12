#include <cstdint>
#include "globals.h"
#include "hook.h"

// Copies the two dwords of the pointed-to pair into this+0x1AC and this+0x1B0.
// Original: __thiscall, one stack arg, ret 4. Returns the second dword (left in eax).
static int __fastcall HudAlertIcons_SetObjectOrientation(void* self, void* /*edx*/, uint32_t* a2) {
    CSF_AT(uint32_t, self, 0x1AC) = a2[0];
    int result = static_cast<int>(a2[1]);
    CSF_AT(uint32_t, self, 0x1B0) = static_cast<uint32_t>(result);
    return result;
}

HOOK(0x494B20, HudAlertIcons_SetObjectOrientation, "auto");
