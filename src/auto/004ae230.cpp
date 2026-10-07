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

// Vtable 0x00839118 slot: returns the constant name string "TMovilAPunto".
// Does not touch `this` (ecx/edx are ignored by the original).
static const char* __fastcall TMovilAPunto_GetName(TMovilAPunto* self, void* /*edx*/)
{
    (void)self;
    return CSF_STR(0x0083919C, "TMovilAPunto");
}

HOOK(0x004AE230, TMovilAPunto_GetName, "auto");
