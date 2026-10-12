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

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(unsigned int, __fastcall, C_00869cd0_Destroy_7563B0, 0x007563B0, (C_00869cd0* self, void*));

static uint32_t* __fastcall C_00840de4_Destroy(C_00840de4* self, void* /*edx*/, char freeSelfFlag)
{
    *(void***)self = CSF_VTBL(0x00840DE4);
    C_00869cd0_Destroy_7563B0(reinterpret_cast<C_00869cd0*>(self), nullptr);
    if ((freeSelfFlag & 1) != 0)
        Mem_Free(self);
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x51B630, C_00840de4_Destroy, "auto");
