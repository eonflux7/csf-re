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

// Returns the type-name literal for this class ("TCombate").
// Original:  mov eax, 0x83A93C ; ret
static const char* __cdecl TCombate_GetTypeName()
{
    return CSF_STR(0x83A93C, "TCombate");
}

HOOK(0x4B94B0, TCombate_GetTypeName, "auto");
