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

CSF_FUNC(int, __fastcall, C_0082e664_BaseCleanup, 0x0041E920, (void* self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static uint32_t* __fastcall C_0082e6a0_Destroy(C_0082e6a0* self, void* /*edx*/, char freeSelfFlag)
{
    *(void***)self = CSF_VTBL(0x0082E6A0);
    C_0082e664_BaseCleanup(self, nullptr);
    if ((freeSelfFlag & 1) != 0)
        Mem_Free(self);
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x41EC90, C_0082e6a0_Destroy, "auto");
