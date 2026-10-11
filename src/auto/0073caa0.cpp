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

// Function-pointer type for kernel32!GetSystemTimeAsFileTime (stdcall, one pointer argument).
using GetSystemTimeAsFileTimeFn = void(__stdcall*)(void* lpFileTime);

// Import slot at 0x82D220 holds the address of kernel32!GetSystemTimeAsFileTime.
CSF_GLOBAL(GetSystemTimeAsFileTimeFn, g_GetSystemTimeAsFileTime, 0x82D220);

// Writes the current system time, in units of 10 ms (100 ns FILETIME ticks / 0x2710),
// into the 64-bit value pointed to by `result`, and returns `result`.
static uint64_t* __stdcall NetSession_WriteCurrentMillis(uint64_t* result) {
    // FILETIME is 8 bytes; the callee fills all of them.
    alignas(8) uint8_t fileTimeBytes[8];
    g_GetSystemTimeAsFileTime(fileTimeBytes);

    uint64_t fileTime;
    std::memcpy(&fileTime, fileTimeBytes, sizeof(fileTime));

    *result = fileTime / 0x2710;
    return result;
}

HOOK(0x73CAA0, NetSession_WriteCurrentMillis, "auto");
