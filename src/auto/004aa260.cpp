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

// Returns the literal name of the "TLookAt" task type.
static const char* __cdecl GetTLookAtName()
{
    return CSF_STR(0x83869C, "TLookAt");
}

HOOK(0x4AA260, GetTLookAtName, "auto");
