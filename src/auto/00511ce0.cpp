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

CSF_FUNC(int*, __fastcall, CMenuManager_ReleaseResources, 0x0050F540, (void* self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static uint32_t* __fastcall C_008407a8_Destroy(C_008407a8* self, void* /*edx*/, char freeSelfFlag)
{
    *(void***)self = CSF_VTBL(0x008407A8);
    CMenuManager_ReleaseResources(self, nullptr);
    if ((freeSelfFlag & 1) != 0)
        Mem_Free(self);
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x511CE0, C_008407a8_Destroy, "auto");
