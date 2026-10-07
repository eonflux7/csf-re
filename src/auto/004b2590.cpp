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

// Returns the constant class/state name string "TDistraido" (address 0x0083A2CC).
static const char* __fastcall TDistraido_GetName(TDistraido* self, void* /*edx*/)
{
    (void)self;
    return CSF_STR(0x0083A2CC, "TDistraido");
}

HOOK(0x4B2590, TDistraido_GetName, "auto");
