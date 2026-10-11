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

CSF_FUNC(void, __fastcall, C_00830030_InitVtbl, 0x00428580, (C_00830030* self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static uint32_t* __fastcall C_008300c0_Destroy(C_008300c0* self, void* /*edx*/, char freeSelfFlag)
{
    *(void***)self = CSF_VTBL(0x008300C0);
    C_00830030_InitVtbl(self, nullptr);
    if ((freeSelfFlag & 1) != 0)
        Mem_Free(self);
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x4293F0, C_008300c0_Destroy, "auto");
