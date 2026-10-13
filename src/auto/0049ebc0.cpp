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

static int __fastcall C_00833908_StoreSetupPropertyB(C_00833908* self, void* /*edx*/, int value)
{
    CSF_AT(int, self, 0x188) = value;
    return value;
}

HOOK(0x0049EBC0, C_00833908_StoreSetupPropertyB, "auto");
