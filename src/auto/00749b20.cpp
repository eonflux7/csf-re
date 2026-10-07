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

// Reads the byte flag stored at the start of the object, clears it, and
// returns the value that was read (an atomic-like "take and reset" of a
// one-byte flag/member).
static char __fastcall C_749b20_TakeByteFlag(void* self, void* /*edx*/)
{
    char* flag = reinterpret_cast<char*>(self);
    char previous = *flag;
    *flag = 0;
    return previous;
}

HOOK(0x00749B20, C_749b20_TakeByteFlag, "auto");
