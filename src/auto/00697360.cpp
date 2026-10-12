#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// COptions* g_pOptions. Declared as void* because COptions is not a declared class here.
CSF_GLOBAL(void*, g_pOptions, 0x008BAF8C);

// Stores the active context id in the options object at +0x64 (dword index 25) and returns it.
static int __cdecl SceneLoader_SetActiveContext(int a1) {
    CSF_AT(int, g_pOptions, 0x64) = a1;
    return a1;
}

HOOK(0x00697360, SceneLoader_SetActiveContext, "auto");
