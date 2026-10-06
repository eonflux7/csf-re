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

// 0x0047F5D0 (6 bytes) - returns the address of the literal string "TAtacarRafaga".
// The original only does `mov eax, 0x833000; ret`, so the returned pointer must be
// exactly 0x00833000 (callers may compare the pointer identity).
static const char* __cdecl GetTAtacarRafagaString()
{
    return reinterpret_cast<const char*>(0x00833000u);
}

HOOK(0x47F5D0, GetTAtacarRafagaString, "auto");
