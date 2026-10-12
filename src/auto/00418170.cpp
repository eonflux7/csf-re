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

CSF_FUNC(int, __fastcall, C_0082e128_ReleaseBaseState, 0x00416250, (void* self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static uint32_t* __fastcall C_0082e128_Destroy(C_0082e128* self, void* /*edx*/, char freeSelfFlag)
{
    *(void***)self = CSF_VTBL(0x0082E128);
    C_0082e128_ReleaseBaseState(self, nullptr);
    if ((freeSelfFlag & 1) != 0)
        Mem_Free(self);
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x418170, C_0082e128_Destroy, "auto");
