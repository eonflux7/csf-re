// Reimplementation of 0x0047B0E0 — returns the literal "TModoNinguno".
// Original:
//   0047b0e0  mov eax, 0x831f30   ; pointer to the string in .rdata
//   0047b0e5  ret
// No stack args, so the callee pops 0 bytes (__cdecl).

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

// Address of the NUL-terminated "TModoNinguno" literal inside the module's .rdata.
static const uintptr_t kModoNingunoStringAddr = CSF_ADDR_DATA(0x00831F30);

// Returns the mode token string "TModoNinguno".
static const char* __cdecl GetModoNingunoString()
{
    return reinterpret_cast<const char*>(kModoNingunoStringAddr);
}

HOOK(0x47B0E0, GetModoNingunoString, "auto");
