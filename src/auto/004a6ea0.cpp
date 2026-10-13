#include "hook.h"

// Returns a constant error count (5). The original takes no arguments and never reads ecx.
static int __cdecl AiMountTasks_GetRtcErrorCount() {
    return 5;
}

HOOK(0x4A6EA0, AiMountTasks_GetRtcErrorCount, "auto");
