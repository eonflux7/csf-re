#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"

CSF_GLOBAL(float, g_TransitionOne, 0x82D4D4);

// Returns 1 when the float stored at +0x9C is strictly greater than g_TransitionOne (1.0f), otherwise 0.
static int __fastcall TargetRecord_IsFieldAboveTransitionOne(float* self, void* /*edx*/) {
    return self[39] > g_TransitionOne ? 1 : 0;
}

HOOK(0x4ADB50, TargetRecord_IsFieldAboveTransitionOne, "auto");
