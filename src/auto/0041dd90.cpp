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

CSF_FUNC(int, __fastcall, sub_40FDF0, 0x0040FDF0, (void* self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static uint32_t* __fastcall C_0082e4c0_Destroy(C_0082e4c0* self, void* /*edx*/, char freeSelfFlag)
{
    *(void***)self = CSF_VTBL(0x0082E4C0);
    sub_40FDF0(self, nullptr);
    if ((freeSelfFlag & 1) != 0)
        Mem_Free(self);
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x41DD90, C_0082e4c0_Destroy, "auto");
