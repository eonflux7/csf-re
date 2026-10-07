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

// Returns the name of the "TMove" animation/task identifier.
// Original: mov eax, 0x83860C ; ret
static const char* __cdecl GetTaskName_TMove()
{
    return CSF_STR(0x83860C, "TMove");
}

HOOK(0x4AA250, GetTaskName_TMove, "auto");
