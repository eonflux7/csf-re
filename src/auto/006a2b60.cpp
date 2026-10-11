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

CSF_FUNC(uint8_t*, __cdecl, sub_694BF0, 0x00694BF0, (uint8_t* a1, char a2, char a3));

// Copies a path into a 200-byte local, rewrites separators ('/' = 47 -> '\' = 92)
// via sub_694BF0, then copies the result into the caller's destination buffer.
static int __stdcall ZlibLog_CopyPathBackslashed(char* destination, const char* source) {
    char pathBuffer[200];

    strcpy(pathBuffer, source);
    sub_694BF0(reinterpret_cast<uint8_t*>(pathBuffer), 47, 92);
    strcpy(destination, pathBuffer);
    return 0;
}

HOOK(0x006A2B60, ZlibLog_CopyPathBackslashed, "auto");
