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

CSF_FUNC(int, __fastcall, C_008311f0_ReleaseBase, 0x0044DB40, (void* self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static int* __fastcall C_00831210_Destroy(C_00831210* self, void* /*edx*/, char freeSelfFlag)
{
    *(void***)self = CSF_VTBL(0x00831210);
    C_008311f0_ReleaseBase(self, nullptr);
    if ((freeSelfFlag & 1) != 0)
        Mem_Free(self);
    return reinterpret_cast<int*>(self);
}

HOOK(0x44E0A0, C_00831210_Destroy, "auto");
