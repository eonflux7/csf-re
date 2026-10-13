#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// _RTC_NumErrors_0: the RTC runtime-check error counter stub. The original
// always returns 4 and never reads ecx, so it is a plain cdecl function.
static int __cdecl AiMountTasks_GetRtcNumErrors() {
    return 4;
}

HOOK(0x4A7DA0, AiMountTasks_GetRtcNumErrors, "auto");
