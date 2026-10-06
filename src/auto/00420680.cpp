// Reimplementation of GetTAturdidoString at 0x00420680.
// Returns the constant string "TAturdido" (pointer to the original data at 0x0082EAD8).

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

// 0x00420680: returns a pointer to the "TAturdido" string constant.
static const char* __cdecl GetTAturdidoString()
{
    return CSF_STR(0x0082EAD8, "TAturdido");
}

HOOK(0x00420680, GetTAturdidoString, "auto");
