#include "globals.h"
#include "hook.h"

CSF_GLOBAL(char, g_GlobalObjectMode, 0x8A8925);

static char __stdcall GlobalObject_ApplyMode(char mode) {
    g_GlobalObjectMode = mode;
    return mode;
}

HOOK(0x73C7F0, GlobalObject_ApplyMode, "auto");
