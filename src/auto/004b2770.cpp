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

CSF_FUNC(void, __fastcall, C_0082d410_SetVtable, 0x004DBD40, (C_0082d410* self, void*));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

static C_0082d410* __fastcall C_0082d410_Destroy(C_0082d410* self, void* /*edx*/, char freeSelfFlag)
{
    CSF_AT(void*, self, 0x14) = CSF_VTBL(0x0082D410);
    C_0082d410_SetVtable(self, nullptr);
    if ((freeSelfFlag & 1) != 0)
        CoreMemory_Free(self);
    return self;
}

HOOK(0x004B2770, C_0082d410_Destroy, "auto");
