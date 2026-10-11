#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"

CSF_GLOBAL(float, g_fZero, 0x82D4CC);

// Returns 1 when the float stored at +0x9C is strictly greater than 0.0f, otherwise 0.
static int __fastcall HudAlertIcons_IsFieldAboveZero(float* self, void* /*edx*/) {
    return self[39] > g_fZero ? 1 : 0;
}

HOOK(0x4A1960, HudAlertIcons_IsFieldAboveZero, "auto");
