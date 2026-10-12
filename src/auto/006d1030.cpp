#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Stores newContext at +0x9C only when it differs from the current value, and returns newContext.
static int __fastcall SceneLoader_SetClumpContext(void* self, void* /*edx*/, int newContext) {
    int result = newContext;
    if (CSF_AT(int, self, 0x9C) != newContext)
        CSF_AT(int, self, 0x9C) = newContext;
    return result;
}

HOOK_KEEP_ECX(0x006D1030, SceneLoader_SetClumpContext, 1, "gfx_lights");
