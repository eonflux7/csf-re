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

// Table of const char* entries; the original indexes it directly by a1 (stride 4).
CSF_GLOBAL(const char*, g_psz_DISTRACCION, 0x8997F0);

// Returns the string-table entry at index a1 (__cdecl, no stack cleanup).
static const char* __cdecl GetFutureErrorWhatString(int index) {
    return *(&g_psz_DISTRACCION + index);
}

HOOK(0x4E5750, GetFutureErrorWhatString, "auto");
